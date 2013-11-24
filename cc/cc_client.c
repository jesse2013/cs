/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#include "cc_client.h"


void *receive_routine(void *p)
{
    DFE;
    if (p == NULL) {
        E("parameter error.");
        pthread_exit((void *)0);
    }
    int fd = *(int *)p;

	size_t bl = 512;
	char *b = (char *)cs_malloc(sizeof(char) * bl);
	if (b == NULL) {
		E("cs_malloc() failed.");
        pthread_exit((void *)0);
        return NULL;
	}

	ssize_t s = 0;
    while (1) {
        s = read(fd, b, bl);
        if (s < 0) {
            E("%s", strerror(errno));
            cs_free(&b);
            return NULL;
        } else if (s == 0) {
            D("server is closed."); 
            cs_free(&b);
            pthread_exit((void *)0);
            return NULL;
        }

        D(GREEN"receive %s %d bytes."NO, b, strlen(b));
		memset(b, '\0', bl);
    }
    DFL;
}


int main(int argc, char *argv[])
{
	int sockfd = -1;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		E("%s", strerror(errno));
		return -1;
	}
	
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	memset(&addr, '\0', addrlen);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

	int ret = connect(sockfd, (struct sockaddr *)&addr, addrlen);
	if (ret == -1) {
		E("%s", strerror(errno));
		return -1;
	}
	char str[INET_ADDRSTRLEN];
	D("connect %s at PORT %d success.", 
      inet_ntop(AF_INET, &addr.sin_addr, str, sizeof(str)), 
      ntohs(addr.sin_port));

	size_t buflen = 512;
	char *buf = (char *)cs_malloc(sizeof(char) * buflen);
	if (buf == NULL) {
		E("cs_malloc() failed.");
		return -1;
	}

    pthread_t thread;
    ret = pthread_create(&thread, NULL, receive_routine, &sockfd);

	ssize_t s = 0;
	while (1) {
        scanf("%s", buf);

		s = write(sockfd, buf, strlen(buf));
		if (s == -1) {
			E("%s", strerror(errno));
			cs_free(&buf);
			return -1;
		}
        D(GREEN"send %s %d bytes."NO, buf, strlen(buf));

        memset(buf, '\0', buflen);
	}

    pthread_join(thread, NULL);
	cs_free(&buf);

	ret = close(sockfd);
	if (ret == -1) {
		E("%s", strerror(errno));
		return -1;
	}

	return 0;
}
