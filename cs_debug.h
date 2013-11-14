/************************************************************
 * Email: troyyuan@smit.com.cn
 * Date: 2013-11-14 21:36:56
 * *********************************************************/

#ifndef _CS_DEBUG_H_INCLUDED_
#define _CS_DEBUG_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h> //int8_t uint8_t uint16_t uint32_t
#include <errno.h>
#include <assert.h>
#include <time.h> //time(2) localtime(3) asctime(3)
#include <sys/time.h> //gettimeofday()
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h> //htonl inet_pton
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <poll.h>
#include <fcntl.h> //manipulate file descriptor
#include <sys/ioctl.h> //control device
#include <net/if.h> //struct ifreq
#include <sys/stat.h> //file info
#include <sys/prctl.h> //prctl()
#include <sys/sysinfo.h> //???
#include <sys/utsname.h> //uname(2) get name and information about current kernel
#include <dirent.h> //opendir closedir
#include <ctype.h> //tolower()
#include <math.h> // -lm
#include <pthread.h> // -lpthread


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


/* time */
char *get_time(void)
{
	static char time[32] = ""; 
	struct timeval tv; 
	time_t s = 0;

	gettimeofday(&tv, NULL);
	s = tv.tv_sec + 3600*8;

	snprintf(time, sizeof(time), "%ld.%02ld:%02ld:%02ld.%03d", 
            s/(3600*24) - 16023, s/3600%24, s/60%60, s%60, 
            (int)tv.tv_usec/1000);

	return time;
}

long long get_ms(void)
{ 
    long long ms = 0;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    ms = tv.tv_sec * 1000ULL + tv.tv_usec / 1000;

    return ms;
}


/* thread name */
void set_threadname(char *name)
{ 
    if (name == NULL)
        return;

    int len = strlen(name);
    if (len > 15) 
        name += len - 15;

#ifndef __APPLE__
    prctl(PR_SET_NAME, (unsigned long)name);
#else
    pthread_setname_np(name);
#endif
}

char *get_threadname(void)
{ 
    static char name[32] = "";

#ifndef __APPLE__
    prctl(PR_GET_NAME, (unsigned long)name);
#else
    pthread_getname_np(pthread_self(), name, sizeof(name));
#endif

    return name;
}


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
