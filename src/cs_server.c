/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#include "cs_server.h"


void cst_init(cs_t *cst)
{
    cst->serv_fd = -1;

    //cst->cli_fd = {-1};
    //cst->cli_buf = NULL;

    cst->nfds = -1;;
    //cst->readfds
    //cst->writefds
    //cst->exceptfds

    //cst->db = NULL;
}

void cst_free(cs_t *cst)
{
}

int cst_accept(int i)
{
    /*
    struct sockaddr_in peer_addr;
    socklen_t peer_addrlen = sizeof(peer_addr);

    memset(&peer_addr, '\0', peer_addrlen);

    size_t buflen = 512;
    char *buf = (char *)cs_malloc(sizeof(char) * buflen);
    if (buf == NULL) {
        E("cs_malloc() failed.");
        return -1;
    }
    cs_free(&buf);

    D("received from %s at PORT %d",
      inet_ntop(AF_INET, &peer_addr.sin_addr, str, sizeof(str)), 
      ntohs(peer_addr.sin_port));
      */
    return -1;
}

int cst_routine(int i)
{
    return 0;
}

int main(int argc, char *argv[])
{
    cs_t cst;
    cst_init(&cst);

    cst.serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (cst.serv_fd == -1) {
        E("%s", strerror(errno));
        return -1;
    }

    int optval = 1;
    int ret = setsockopt(cst.serv_fd, SOL_SOCKET, SO_REUSEADDR,
            &optval, sizeof(optval));
    if (ret == -1) {
        E("%s", strerror(errno));
        return -1;
    }

#ifdef SO_REUSEPORT
    optval = 1;
    ret = setsockopt(cst.serv_fd, SOL_SOCKET, SO_REUSEPORT,
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
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(cst.serv_fd, (struct sockaddr *)&addr, addrlen);
    if (ret == -1) {
        E("%s", strerror(errno));
        return -1;
    } else
        D("bind 127.0.0.1:8888 successfully.");

    ret = listen(cst.serv_fd, cst.listen_num);
    if (ret == -1) {
        E("%s", strerror(errno));
        return -1;
    }

    FD_ZERO(&cst.readfds);
    FD_ZERO(&cst.writefds);
    FD_ZERO(&cst.exceptfds);

    int n = 0;
    int i = 0;
    while (1) {
        n = select(cst.nfds + 1, &cst.readfds, &cst.writefds,
                &cst.exceptfds, &cst.timeout);
        if (n < 0) {
            if (errno == EINTR)
                continue;

            E("select() faile.");
            break;
        } else if (n == 0) {
            D("timeout, nothing to be done.");
        } else {
            for (i = 0; i <= cst.nfds; i++) {
                if (FD_ISSET(i, &cst.readfds)) {
                    if (i == cst.serv_fd)
                        cst_accept(i);
                    else
                        cst_routine(i);
                } else if (FD_ISSET(i, &cst.writefds)) {
                    D("write occurrence.");
                } else
                    E("except occurrence.");
            }
        }
    }

    cst_free(&cst);
    return 0;
}
