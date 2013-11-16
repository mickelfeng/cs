/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#include "cs_server.h"


int main(int argc, char *argv[])
{
    D("hello world.");
#if 1
	int socket(int domain, int type, int protocol);
	int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
	int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	int listen(int sockfd, int backlog);
	int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	ssize_t read(int fd, void *buf, size_t count);
	ssize_t write(int fd, const void *buf, size_t count);
	ssize_t read(int fd, void *buf, size_t count);
	int close(int fd);
#endif
    return 0;
}
