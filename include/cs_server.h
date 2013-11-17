/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#ifndef _CS_SERVER_H_INCLUDED_
#define _CS_SERVER_H_INCLUDED_


#include <sqlite3.h>

#include "cs_debug.h"

typedef struct {
	char *name;
	char *passwd;
	char *datetime;
	char *buddy_name;
	char *content;
	int req_type;
} cs_request_t;


typedef struct {
	char *name;
	char *passwd;
} cs_user_t;

typedef struct {
	char *name;
	char *online;
} cs_buddy_t;

typedef struct {
	char *datetime;
	char *name;
	char *content;
} cs_his_t;


#endif /* _CS_SERVER_H_INCLUDED_ */
