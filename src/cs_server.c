/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#include "cs_server.h"


int main(int argc, char *argv[])
{
    D("hello world.");

	int socket(int domain, int type, int protocol);
	int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
	int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	int listen(int sockfd, int backlog);
	int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	ssize_t read(int fd, void *buf, size_t count);
	ssize_t write(int fd, const void *buf, size_t count);
	ssize_t read(int fd, void *buf, size_t count);
	int close(int fd);

#if 0
	rscallback();
	sqlite3_open();
	sqlite3_exec(); /* check username & passwd */
	sqlite3_close();

	void FD_CLR(int fd, fd_set *set);
	int  FD_ISSET(int fd, fd_set *set);
	void FD_SET(int fd, fd_set *set);
	void FD_ZERO(fd_set *set);
	int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
#endif

    return 0;
}
