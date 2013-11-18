/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#include "cc_client.h"


int main(int argc, char *argv[])
{
	D("hello world.");

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
	D("received from %s at PORT %d", 
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
	char *req = ":troy:troy:20131117100404:ivy:hello world.";
	//while (1) {
		strncpy(buf, req, strlen(req));
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
		DS(buf);
		memset(buf, '\0', buflen);

		//sleep(1);
	//}

	cs_free(&buf);

	ret = close(sockfd);
	if (ret == -1) {
		E("%s", strerror(errno));
		return -1;
	}

	return 0;
}
