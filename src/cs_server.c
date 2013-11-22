/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#include "cs_server.h"


#define PORT        8888


sockfd_buf_t **sockfd_list = NULL;
int sockfd_list_num = 0;

int maxfd = 0;
fd_set rfds_g, wfds_g, efds_g;


int sockfd_list_init(int n)
{
    sockfd_list = (sockfd_buf_t **)cs_malloc(sizeof(sockfd_buf_t *) * n);
    if (sockfd_list == NULL) {
        E("cs_malloc() failed.");
        return -1;
    }

    int i = 0;
    for (; i < n; i++)
        sockfd_list[i] = NULL;

    sockfd_list_num = n;
    return 0;
    
}

void register_readfd(int fd)
{
    FD_SET(fd, &rfds_g);

    if (maxfd < fd)
        maxfd = fd;
}

void register_writefd(int fd)
{
    FD_SET(fd, &wfds_g);

    if (maxfd < fd)
        maxfd = fd;
}

void register_exceptfd(int fd)
{
    FD_SET(fd, &efds_g);

    if (maxfd < fd)
        maxfd = fd;
}


buf_t buf_init(void)
{
    buf_t buf = {NULL, 0, 0};

    buf.data = (char *)cs_malloc(sizeof(char) * BUF_MAX);
    if (buf.data == NULL) {
        E("cs_malloc() failed.");
        return buf;
    }

    buf.len = 0;
    buf.max = BUF_MAX;

    return buf;
}

sockfd_buf_t *sockfd_buf_init(void)
{
    sockfd_buf_t *rwbuf = (sockfd_buf_t *)cs_malloc(sizeof(sockfd_buf_t));
    if (rwbuf == NULL) {
        E("cs_malloc() failed.");
        return NULL;
    }

    rwbuf->rbuf = buf_init();
    rwbuf->wbuf = buf_init();

    if (rwbuf->rbuf.data == NULL || rwbuf->wbuf.data == NULL) {
        E("buf_init() failed.");
        cs_free(&rwbuf->wbuf);
        cs_free(&rwbuf->rbuf);
        cs_free(&rwbuf);
        return NULL;
    }

    return rwbuf;
}

int cs_accept(int servfd)
{
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    memset(&addr, '\0', addrlen);

    int clifd = accept(servfd, (struct sockaddr *)&addr, &addrlen);
    if (clifd < 0) {
        E("accept() failed.");
        return -1;
    }

	char ip_str[INET_ADDRSTRLEN];
    D("accept connect come from %s port %d",
      inet_ntop(AF_INET, &addr.sin_addr, ip_str, sizeof(ip_str)), 
      ntohs(addr.sin_port));

    sockfd_buf_t *rwbuf = sockfd_buf_init();
    if (rwbuf == NULL) {
        E("sockfd_buf_init() failed.");
        return -1;
    }

    sockfd_list[clifd] = rwbuf;
    register_readfd(clifd);

    return 0;
}

int cs_routine(int fd)
{
    sockfd_buf_t *rwbuf = sockfd_list[fd];

    if (rwbuf == NULL)
        return -1;
    if (rwbuf->rbuf.data == NULL)
        return -1;
    if (rwbuf->wbuf.data == NULL)
        return -1;

    int n = read(fd, rwbuf->rbuf.data, rwbuf->rbuf.max);
    if (n <= 0) {
        E("%s", strerror(errno));
        return -1;
    }
    rwbuf->rbuf.len = n;

    D("received %s %d bytes from %d.", rwbuf->rbuf.data, n, fd);

    // FIXME: return value type
    sql_routine(rwbuf);

    n = write(fd, rwbuf->wbuf.data, rwbuf->wbuf.max);
    if (n == -1) {
        E("%s", strerror(errno));
        return -1;
    }

    memset(rwbuf->wbuf.data, 0, rwbuf->wbuf.max);
    memset(rwbuf->rbuf.data, 0, rwbuf->rbuf.max);

    return 0;
}


int main(int argc, char *argv[])
{
    int ret = sockfd_list_init(1024);
    if (ret < 0) {
        E("sockfd_list_init() failed.");
        return -1;
    }

    int servfd = socket(AF_INET, SOCK_STREAM, 0);
    if (servfd == -1) {
        E("%s", strerror(errno));
        return -1;
    }

    int optval = 1;
    ret = setsockopt(servfd, SOL_SOCKET, SO_REUSEADDR,
            &optval, sizeof(optval));
    if (ret == -1) {
        E("%s", strerror(errno));
        return -1;
    }

#ifdef SO_REUSEPORT
    optval = 1;
    ret = setsockopt(servfd, SOL_SOCKET, SO_REUSEPORT,
            &optval, sizeof(optval));
    if (ret == -1) {
        E("%s", strerror(errno));
        return -1;
    }
#endif

    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    memset(&addr, '\0', addrlen);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(servfd, (struct sockaddr *)&addr, addrlen);
    if (ret == -1) {
        E("%s", strerror(errno));
        return -1;
    }

    int backlog = 20;
    ret = listen(servfd, backlog);
    if (ret == -1) {
        E("%s", strerror(errno));
        return -1;
    }

    FD_ZERO(&rfds_g);
    FD_ZERO(&wfds_g);
    FD_ZERO(&efds_g);

    register_readfd(servfd);

    fd_set rfds, wfds, efds;

    struct timeval timeout;

    int n = 0;
    int i = 0;

    D("cs server start 0.0.0.0 %d", PORT);
    while (1) {
        rfds = rfds_g;
        wfds = wfds_g;
        efds = efds_g;

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        n = select(maxfd + 1, &rfds, &wfds, &efds, &timeout);
        if (n < 0) {
            if (errno == EINTR)
                continue;
            E("select() faile.");
            break;
        } else if (n == 0) {
            D("timeout, nothing to be done.");
        } else {
            for (i = 0; i <= maxfd; i++) {
                if (FD_ISSET(i, &rfds)) {
                    if (i == servfd)
                        cs_accept(i);
                    else
                        cs_routine(i);
                } else if (FD_ISSET(i, &wfds)) {
                    D("write occurrence.");
                } else if (FD_ISSET(i, &efds)) {
                    E("except occurrence.");
                } else {
                    //E("undefined occurrence.");
                }
            }
        }
    }

    return 0;
}
