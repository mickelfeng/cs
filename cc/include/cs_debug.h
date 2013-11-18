/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#ifndef _CS_DEBUG_H_INCLUDED_
#define _CS_DEBUG_H_INCLUDED_


#include "cs_common.h"


#define NO          "\033[0m"
#define BROWN       "\033[0;33m"  /* zong */
#define YELLOW      "\033[1;33m"
#define WHITE       "\033[1;37m"
#define RED         "\033[0;31m"
#define LRED        "\033[1;31m"
#define GREEN       "\033[0;32m"
#define LGREEN      "\033[1;32m"
#define BLUE        "\033[0;34m"
#define LBLUE       "\033[1;34m"
#define GRAY        "\033[1;30m"
#define LGRAY       "\033[0;37m"
#define CYAN        "\033[0;36m"  /* qing */
#define LCYAN       "\033[1;36m"
#define PURPLE      "\033[0;35m"  /* zi */
#define LPURPLE     "\033[1;35m"


#define ASSERT(x) assert(x)
#define MIN(x, y) ((x)<(y)?(x):(y))
#define MAX(x, y) ((x)>(y)?(x):(y))


/* inet_network */
#define FORMAT "%d.%d.%d.%d"
#define SPLITIP(ip) ((ip)>>24)&0xFF, ((ip)>>16)&0xFF, ((ip)>>8)&0xFF, (ip)&0xFF
#define DIP(x) D(#x"="FORMAT, SPLITIP(x))


typedef char bool;
#define true    1
#define false   0


#define P(fmt, args...) { \
	fprintf(stdout, NO"[%s]:%s->%s->%s:%05d --> "fmt"\n"NO, \
			get_time(), get_threadname(), __FILE__, __FUNCTION__, __LINE__, ##args); }

#define E(fmt, args...) P(fmt, ##args)

#ifdef DEBUG /* -DDEBUG */
#define D(fmt, args...) P(fmt, ##args)
#else
#define D(fmt, args...)
#endif

#define DC(x)       D(#x"=%c", x)
#define DS(x)       D(#x"=%s", (char *)(x))
#define DP(x)       D(#x"=%p", x)

#define DF(x)       D(#x"=%f", x)
#define DLF(x)      D(#x"=%lf", x) /* double */

#define DD(x)       D(#x"=%d", (int)(x))
#define DLD(x)      D(#x"=%ld", x)
#define DLLD(x)     D(#x"=%lld", x)

#define DU(x)       D(#x"=%u", x) /* unsigned int */
#define DLU(x)      D(#x"=%lu", x) /* long unsigned int */
#define DLLU(x)     D(#x"=%llu", x)

#define DO(x)       D(#x"=%o", x) 
#define DX(x)       D(#x"=0x%X", x)

#define DL          DD(__LINE__)
#define DFE         D("*****%s() Enter*****", __FUNCTION__)
#define DFL         D("*****%s() Leave*****", __FUNCTION__)

typedef struct {
	size_t      len;
	char       *data;
} str_t;

#define DSTR(len, data)     D(#len"=%u, "#data"=%.*s", len, len, data)
#define DDSTR(x)            D(#x".len=%u, "#x".data=%.*s", x.len, x.len, x.data)
#define DPSTR(x)            D(#x"->len=%u, "#x"->data=%.*s", x->len, x->len, x->data)

#define DPSTRIF(x) { \
	if (x != NULL) { D(#x": len=%u, data=%.*s", x->len, x->len, x->data); }\
	else { DP(x); } }

#define DVBUF(len, data) { \
	if(len > 0 && data != NULL) \
    { \
        int loop = 0; \
        char str[1024] = {'\0'}; \
        for(loop=0; loop<len && loop<sizeof(str)/3; loop++) \
        { snprintf(str+loop*3, sizeof(str)-loop*3, "%02X ", ((uint8_t *)data)[loop]); } \
        D(#len"=%u, "#data"=%p, BUF=%s", len, data, str); \
    } \
	else \
    { DD(len); DP(data); } }

#define DDBUF(x)    DVBUF(x.len, x.data)
#define DPBUF(x)    DVBUF(x->len, x->data)


#endif /* _CS_DEBUG_H_INCLUDED_ */
