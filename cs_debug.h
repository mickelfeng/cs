/************************************************************
 * Copyright (c) 2013, SMIT Beijing
 * All Rights Reserved.
 * Email: troyyuan@smit.com.cn
 * Date: 2013-07-28 02:28:18
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
//#include <curl/curl.h> // -lcurl
//#include <event2/event.h> // -levent
//#include <openssl/md5.h> // -lcrypto


#define NO			"\033[0m"
#define BROWN		"\033[0;33m"  /* zong */
#define YELLOW		"\033[1;33m"
#define WHITE		"\033[1;37m"
#define RED			"\033[0;31m"
#define LRED		"\033[1;31m"
#define GREEN		"\033[0;32m"
#define LGREEN		"\033[1;32m"
#define BLUE		"\033[0;34m"
#define LBLUE		"\033[1;34m"
#define GRAY		"\033[1;30m"
#define LGRAY		"\033[0;37m"
#define CYAN		"\033[0;36m"  /* qing */
#define LCYAN		"\033[1;36m"
#define PURPLE		"\033[0;35m"  /* zi */
#define LPURPLE		"\033[1;35m"


#define ASSERT(x) assert(x)
#define MIN(x, y) ((x)<(y)?(x):(y))
#define MAX(x, y) ((x)>(y)?(x):(y))


/* time */
char *get_time(void)
{
	static char time[32] = ""; 
	struct timeval tv; 
	gettimeofday(&tv, NULL);
	uint32_t s = tv.tv_sec+3600*8;
	snprintf(time, sizeof(time), "%d.%02d:%02d:%02d.%03d", s/(3600*24)/*-15900*/, s/3600%24, s/60%60, s%60, (int)tv.tv_usec/1000);
	return time;
}

long long get_ms(void)
{ 
    uint32_t ms = 0;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    ms = tv.tv_sec * 1000ULL + tv.tv_usec / 1000;
    return ms;
}


/* thread name */
void set_threadname(char *name)
{ 
    if(name == NULL) { return; }
    int len = strlen(name);
    if(len > 15) { name += len - 15; }
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

#define DEBUG
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


/* inet_network */
#define FORMAT "%d.%d.%d.%d"
#define SPLITIP(ip) ((ip)>>24)&0xFF, ((ip)>>16)&0xFF, ((ip)>>8)&0xFF, (ip)&0xFF
#define DIP(x) D(#x"="FORMAT, SPLITIP(x))
 

typedef char bool;
#define true    1
#define false   0

typedef struct {
	uint32_t    len;
	char       *data;
    bool        tf;
} str_t;

#define DSTR(len, data)     D(#len"=%u, "#data"=%.*s", len, len, data)
#define DDSTR(x)            D(#x".len=%u, "#x".data=%.*s", x.len, x.len, x.data)
#define DPSTR(x)            D(#x"->len=%u, "#x"->data=%.*s", x->len, x->len, x->data)

#define DPSTRIF(x) { \
	if(x != NULL) { D(#x": len=%u, data=%.*s", x->len, x->len, x->data); }\
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


time_t lms_atoi_starttime(const str_t *string_time, const char *format)
{
    if(string_time == NULL) { E("parameter error"); return 0; }
    if(format == NULL) { format = "%Y%m%d%H%M%S"; }

    time_t sec = 0;
    struct tm tm;
    if(strptime((char *)string_time->data, format, &tm) == NULL)
    {
        E("strptime() failed");
        return 0;
    }
    sec = mktime(&tm);
    //DLD(sec);

    return sec;
}

time_t lms_get_systime(void)
{
    time_t sec = 0;
    time(&sec);
    if(sec <= 0) { E("time() failed"); }
    /*
    struct tm *tm = localtime(&sec);
    char s[15] = {'\0'};
    strftime(s, 15, "%G%m%d%H%M%S", tm);
    DS(s);
    */
    return sec;
}


/* memory */
void *ty_malloc(size_t size)
{
    void *ptr = malloc(size);
    if(ptr != NULL) { memset(ptr, '\0', size); }
    return ptr;
}

void ty_free(void **ptr)
{
    if(*ptr != NULL) { free(*ptr); *ptr = NULL; }
}


/* file & memory */
FILE *lms_mem2file(const char *path, const char *mode, const char *data)
{
    FILE *p = fopen(path, mode);
    if(p == NULL) { E("fopen failed"); return NULL; }
    //size_t n = 
    fwrite(data, strlen(data), 1, p);
    //if(n == 0) { E("fwrite failed"); return NULL; }
    return p;
}
char *lms_file2mem(const int pt, const int du)
{
    return 0;
}


/* directory */
int ty_ckdir(char *dir)
{
	ASSERT(dir != NULL);

	DIR *dp;
	dp = opendir(dir);
	if(dp == NULL)
	{
		switch(errno)
		{
			case EACCES:
				D("Permission denied.");
				return -1;
			case EMFILE:
				D("Too many file descriptors in use by process.");
				return -1;
			case ENFILE:
				D("Too many files are currently open in the system.");
				return -1;
			case ENOENT:
				D("%s is not existent, but created success.", dir);
				ty_mkdir(dir, 0777);
				break;
			case ENOMEM:
				D("Insufficient memory to complete the operation.");
				return -1;
			case ENOTDIR:
				D("name is not a directory.");
				return -1;
			default:
				D("%s is existence.", dir);
				break;
		}
	}
	closedir(dp);
	return 0;
}

int ty_mkdir(char *dir, int perm)
{
	char *dircp = NULL, *dc = NULL;
	int dlen = strlen(dir);

	dircp = dc = (char *)malloc(dlen+2);
	memset(dircp, 0, dlen+2);
	strncpy(dircp, dir, dlen);

	if(dc != NULL && *dc == '/') dc++;
	while(*dc != 0)
	{   
		while(*dc != '/' && *dc != 0) dc++;
		*dc = 0;
		mkdir(dircp, perm);
		*dc = '/';
		dc++;
	}   

	free(dircp);
	return 0;
}

//recursion del dir, rtdir is [dir/NULL]
int ty_rmdir(char *dir, char *rtdir)
{
	ASSERT(dir != NULL);
	struct dirent *dent;
	struct stat st;
	DIR *dp = NULL;

	dp = opendir(dir);
	if(dp == NULL) { E("opendir %s failed.", dir); return -1; }

	chdir(dir);
	while((dent = readdir(dp)) != NULL)
	{
		lstat(dent->d_name, &st);
		if(S_ISDIR(st.st_mode)) //is dir
		{   
			if(strcmp(".", dent->d_name)==0 || strcmp("..", dent->d_name)==0)
				continue;
			ty_rmdir(dent->d_name, rtdir);
		}   
		else if(/*strcmp(dent->d_name, IDX_NAME) == 0 ||*/ dir != rtdir)
		{   
			if(remove(dent->d_name) == -1) 
			{   
				E("remove %s failed.", dent->d_name);
				return -1; 
			}   
		}   
	}   
	chdir("..");

	if(strcmp(dir, rtdir) != 0) //if rtdir = dir, the root directory be saved.
	{
		if(rmdir(dir) == -1) { E("rmdir %s failed.", dir); return -1; }
		else D("rmdir %s success.", dir);
	}
	else
	{
		D("rmdir success, rootdir %s be saved.", rtdir);
	}

	closedir(dp);
	return 0;
}


/* multipurpose one-way list */
// FIXME
#if 0
/*
 * one-way list's function 
 */
void m3u8_list_init(lms_m3u8_list_t *hd)
{
	hd->head = NULL;
	hd->tail= NULL;
	hd->num = 0;
}
void m3u8_list_add_head(lms_m3u8_list_t *hd, lms_m3u8_t *new)
{
	new->next = hd->head;
	hd->head = new;
	hd->num++;
}
void m3u8_list_add_tail(lms_m3u8_list_t *hd, lms_m3u8_t *new)
{
	if(hd->head != NULL)
	{
		hd->tail->next = new;
		hd->tail = new;
	}
	else
	{
		hd->head = hd->tail = new;
	}
	hd->num++;
}
void m3u8_list_display(lms_m3u8_list_t *hd)
{
	lms_m3u8_t *nd = NULL;
	nd = hd->head;
	while(nd != NULL)
	{
        DDSTR(nd->fn);
        DDSTR(nd->ctx);
        DD(nd->save);
		nd = nd->next;
	}
}
lms_m3u8_t *m3u8_list_search(lms_m3u8_list_t *hd, char *name)
{
	if(hd == NULL || name == NULL) { E("parameters error"); return NULL; }

	lms_m3u8_t *nd = NULL;
	nd = hd->head;
	while(nd != NULL)
	{
		if(strncasecmp((char *)nd->fn.data, name, strlen(name)) == 0) { return nd; }
		nd = nd->next;
	}   
    if(nd == NULL) { E("haven't %s", name); }
	return nd;
}
void m3u8_node_free(lms_m3u8_t *nd)
{
    if(nd != NULL)
    {
        if(nd->fn.data != NULL) { lms_free((void **)&nd->fn.data); }
        if(nd->ctx.data != NULL) { lms_free((void **)&nd->ctx.data); }
        lms_free((void **)&nd);
    }
}
void m3u8_list_free(lms_m3u8_list_t *hd)
{
    if(hd == NULL) { return; }

    lms_m3u8_t *nd = NULL;
    nd = hd->head;
    while(nd != NULL)
    {
        m3u8_node_free(nd);
        hd->num--;
		nd = nd->next;
    }
    lms_free((void **)&hd);
}
#endif


/* compute or convert md5 */
#ifdef crypto 
void ty_cpt_md5(vcnt_t *vcnt, char *b16) //16bit bin;8bit hex
{
    if(vcnt != NULL)
    {
        MD5_CTX ctx;
        MD5_Init(&ctx);
        MD5_Update(&ctx, vcnt->pt, vcnt->len);
        MD5_Final((unsigned char *)b16, &ctx);
     }
	D("b16=0x%.*X", 8, b16);
}

void ty_cvt_md5(char *b16, char *b32) //32bit hex
{
	int i;
    for(i=0; i<16; i++)
    {   
        sprintf(b32 + i * 2, "%02x", (unsigned char)b16[i]);
     }
	D("b32=%.*s", 32, b32);
} 
#endif


/* string */
int str_num(const char *url, const char *trait)
{
    int i, j, k, z;
    i = j = k = z = 0;

    for(i=0; url[i]!='\0'; i++)
    {
        for(j=i,k=0; trait[k]!='\0' && url[j]==trait[k]; j++,k++) { ; }
        if(k>0 && trait[k]=='\0') { z++; }
    }

    return z;
}

int str_end(const char *url, const char *trait, const int times)
{
    int i, j, k, z;
    i = j = k = z = 0;

    for(i=0; url[i]!='\0'; i++)
    {
        for(j=i,k=0; trait[k]!='\0' && url[j]==trait[k]; j++,k++) { ; }
        if(k>0 && trait[k]=='\0') { z++; }
        if(z == times) { return i; }
    }

    return -1;
}

char *str_cut(const char *url, const int sp, const int ep)
{
    int str_len = ep + 1 - sp;
    char *str = (char *)ty_malloc(str_len);
    if(str == NULL) { E("malloc failed"); return NULL; }
    memset(str, '\0', str_len);
    memcpy(str, url + sp, ep - sp);
    return str;
}

char *lms_itoa(const int n, int *len)
{
    if(n < 0) { E("parameter error"); return NULL; }

    int m = n;

    int i = 0;
    for(; m; i++)
    {
        m = m / 10;
    }

    if(len != NULL) { *len = i; }

    char *a = (char *)ty_malloc(sizeof(char) * i);
    if(a == NULL) { E("malloc failed"); return NULL; }
    i--;

    m = n;
    for(; m; i--)
    {
        *(a + i) = (m % 10) + '0';
        m = m / 10;
    }

    return a;
}
/*
int lms_regex(const char *str, const char *regex)
{
    if(str == NULL || regex == NULL) { E("parameter error."); return -1; }

    regex_t preg;
    int ret = -1;
    ret = regcomp(&preg, regex, REG_EXTENDED);
    if(ret != 0) { E("regcomp() failed."); return -1; }

    size_t nmatch = 2;
    regmatch_t pmatch[2];
    ret = regexec(&preg, str, nmatch, pmatch, 0);
    if(ret != 0) { E("regexec() failed."); regfree(&preg); return -1; }

    regfree(&preg);
    return ret;
}
*/

/* number */
int lms_cmpt_digit_num(const int n)
{
    if(n <= 0) { E("parameter error"); return 0; }

    int m = n;
    int i = 0;
    for(; m; i++)
    {
        m = m / 10;
    }

    return i;
}


#endif /* _CS_DEBUG_H_INCLUDED_ */
