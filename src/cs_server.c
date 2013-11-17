/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#include "cs_server.h"


void request_init(cs_request_t *req)
{
	req->name = NULL;
	req->passwd = NULL;
	req->datetime = NULL;
	req->buddy_name = NULL;
	req->content = NULL;
	req->req_type = -1;
}

void request_dump(cs_request_t *req)
{
	D("***********************************");
	DSIF(req->name);
	DSIF(req->passwd);
	DSIF(req->datetime);
	DSIF(req->buddy_name);
	DSIF(req->content);
	DD(req->req_type);
	D("***********************************");
}

cs_request_t cs_parse_request(char *buf)
{
	cs_request_t req;
	request_init(&req);

	if (buf == NULL) {
		E("parameter error.");
		return req;
	}

	char *buf_copy = strdup(buf);
	if (buf_copy == NULL) {
		E("strncup() failed.");
		return req;
	}

	char *str = buf_copy;
	char *token = NULL;
	char *saveptr = NULL;
	int i = 0;
	while (1) {
		token = strtok_r(str, ":", &saveptr);
		if (token == NULL)
			break;

		switch (i) {
			case 0:
				req.name = token;
				//DVBUF(strlen(req.name), req.name);
				break;
			case 1:
				req.passwd = token;
				break;
			case 2:
				req.datetime = token;
				break;
			case 3:
				req.buddy_name = token;
				break;
			case 4:
				req.content = token;
				break;
			default:
				DD(i);
				break;
		}

		str = NULL;
		i++;
	}
	//request_dump(&req);

	cs_free(&buf_copy);
	return req;
}

int sql_check_identity_cb(void *p, int argc, char **value, char **name)
{
	*(int *)p = argc;
	return 0;
}

int sql_get_buddy_cb(void *p, int argc, char **value, char **name)
{
	cs_str_t *buddy = (cs_str_t *)p;

	sprintf(buddy->data + buddy->len, ":%s-%s", value[1], value[2]);
	buddy->len = strlen(buddy->data);

	return 0;
}

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

	sqlite3 *db;
	ret = sqlite3_open("./cs_user.db", &db);
	if (ret != SQLITE_OK) {
		E("sqlite3_open() failed.");
		return -1;
	}
	int query_len_max = 512;
	char *query_line = (char *)cs_malloc(sizeof(char) * query_len_max);

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

		// FIXME: why server exit when client ctrl-c
		while (1) {
			memset(buf, '\0', buflen);

			s = read(peer_sockfd, buf, buflen);
			if (s == -1) {
				E("%s", strerror(errno));
				break;
			}
			DS(buf);

			/* check username & passwd */
			cs_request_t req = cs_parse_request(buf);
			if (req.name == NULL) {
				E("cs_parse_request() failed.");
				break;
			}
			// FIXME: why req.name si empty
			//DVBUF(strlen(req.name), req.name);
			request_dump(&req);

			memset(query_line, '\0', query_len_max);
			sprintf(query_line, "select * from user where name='tom' and passwd='%s'", req.passwd);
			DS(query_line);

			ret = sqlite3_exec(db, query_line, sql_check_identity_cb, NULL, NULL);
			if (ret == SQLITE_ABORT) {
				E("sqlite3_exec() failed.");
				break;
			}

			memset(buf, '\0', buflen);
			memset(query_line, '\0', query_len_max);

			/* get buddy name list */
			sprintf(query_line, "select * from troy");
			DS(query_line);

			cs_str_t buddy;
			buddy.data = buf;
			buddy.len = strlen(buf);

			ret = sqlite3_exec(db, query_line, sql_get_buddy_cb, &buddy, NULL);
			if (ret == SQLITE_ABORT) {
				E("sqlite3_exec() failed.");
				break;
			}
			DDSTR(buddy);

			//strncpy(buf, "hello", 5);
			s = write(peer_sockfd, buf, strlen(buf));
			if (s == -1) {
				E("%s", strerror(errno));
				break;
			}
		}
	}

	cs_free(&buf);
	sqlite3_close(db);
	ret = close(sockfd);
	if (ret == -1) {
		E("%s", strerror(errno));
		return -1;
	}

#if 0
	rscallback();
	sqlite3_open();
	sqlite3_exec(); 
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
