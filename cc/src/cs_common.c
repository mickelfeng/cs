/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#include "cs_common.h"


/* memory */
void *cs_malloc(size_t s)
{
    void *p = malloc(s);
    if (p != NULL)
        memset(p, '\0', s);

    return p;
}

void *cs_realloc(void *p, size_t s)
{
    return realloc(p, s);
}

void cs_free(void *p)
{
    void **pp = (void **)p;
    if (*pp != NULL) { 
        free(*pp); 
        *pp = NULL; 
    }
}


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
