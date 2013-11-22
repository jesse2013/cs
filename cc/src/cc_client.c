/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#include "cc_client.h"


int main(int argc, char *argv[])
{
#if 0
    :troy:troy:20131117100404:ivy:hello.
    client向server发送的请求类型
    :0:name:passwd:20131117100404           //注册
    :1:name:passwd:20131117100404           //登录
    :2:20131117100404               //查看所有用户
    :3:ivy:20131117100404:hello world.      //发送聊天消息
    :4:ivy                      //查看聊天记录
#endif

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
			inet_ntop(AF_INET, &addr.sin_addr, str, 
				sizeof(str)), 
			ntohs(addr.sin_port));

	size_t buflen = 512;
	char *buf = (char *)cs_malloc(sizeof(char) * buflen);
	if (buf == NULL) {
		E("cs_malloc() failed.");
		return -1;
	}

	ssize_t s = 0;
	while (1) {
        scanf("%s", buf);
        D(GREEN"sendto server %s %d bytes."NO, buf, strlen(buf));

		s = write(sockfd, buf, strlen(buf));
		if (s == -1) {
			E("%s", strerror(errno));
			cs_free(&buf);
			return -1;
		}
		memset(buf, '\0', buflen);

		s = read(sockfd, buf, buflen);
		if (s == -1) {
			E("%s", strerror(errno));
			cs_free(&buf);
			return -1;
		}
        D(GREEN"recvfrom server %s %d bytes."NO, buf, strlen(buf));
		memset(buf, '\0', buflen);
	}

	cs_free(&buf);

	ret = close(sockfd);
	if (ret == -1) {
		E("%s", strerror(errno));
		return -1;
	}

	return 0;
}
