/************************************************************
 * Email: troyyuan@smit.com.cn
 * Date: 2013-11-14 22:12:34
 * *********************************************************/

#include "cs_debug.h"


/* memory */
void *cs_malloc(size_t s)
{
    void *p = malloc(size);
    if (p != NULL)
        memset(p, '\0', size);

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
