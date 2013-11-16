/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#include "cs_server.h"


int main(int argc, char *argv[])
{
	D("hello world.");

	int sockfd = -1;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		E("%s", strerror(errno));
		return -1;
	}

	int optval = 1;
	int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
			&optval, sizeof(optval));
	if (ret == -1) {
		E("%s", strerror(errno));
		return -1;
	}
	//ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT,
			//&optval, sizeof(optval));
	
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	memset(&addr, '\0', addrlen);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sockfd, (struct sockaddr *)&addr, addrlen);
	if (ret == -1) {
		E("%s", strerror(errno));
		return -1;
	}

	int backlog = 20;
	ret = listen(sockfd, backlog);
	if (ret == -1) {
		E("%s", strerror(errno));
		return -1;
	}

	struct sockaddr_in peer_addr;
	socklen_t peer_addrlen = sizeof(peer_addr);

	memset(&peer_addr, '\0', peer_addrlen);

	size_t buflen = 512;
	char *buf = (char *)cs_malloc(sizeof(char) * buflen);
	if (buf == NULL) {
		E("cs_malloc() failed.");
		return -1;
	}

	int peer_sockfd = -1;
	ssize_t s = 0;
	char str[INET_ADDRSTRLEN];
	while (1) {
		peer_sockfd = accept(sockfd,
				(struct sockaddr *)&peer_addr, &peer_addrlen);
		if (peer_sockfd == -1) {
			E("%s", strerror(errno));
			cs_free(&buf);
			return -1;
		}
		D("received from %s at PORT %d",
				inet_ntop(AF_INET, &peer_addr.sin_addr, str,
					sizeof(str)),
				ntohs(peer_addr.sin_port));

		while (1) {
			s = read(peer_sockfd, buf, buflen);
			if (s == -1) {
				E("%s", strerror(errno));
				cs_free(&buf);
				return -1;
			}
			DS(buf);
			memset(buf, '\0', buflen);

			strncpy(buf, "hello", 5);
			s = write(peer_sockfd, buf, strlen(buf));
			if (s == -1) {
				E("%s", strerror(errno));
				cs_free(&buf);
				return -1;
			}
			memset(buf, '\0', buflen);
		}
	}

	cs_free(&buf);

	ret = close(sockfd);
	if (ret == -1) {
		E("%s", strerror(errno));
		return -1;
	}

#if 0
	rscallback();
	sqlite3_open();
	sqlite3_exec(); /* check username & passwd */
	sqlite3_close();

	void FD_CLR(int fd, fd_set *set);
	int  FD_ISSET(int fd, fd_set *set);
	void FD_SET(int fd, fd_set *set);
	void FD_ZERO(fd_set *set);
	int select(int nfds, fd_set *readfds, fd_set *writefds,
			fd_set *exceptfds, struct timeval *timeout);
#endif

	return 0;
}
