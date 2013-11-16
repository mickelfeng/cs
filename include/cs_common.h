/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#ifndef _CS_COMMON_H_INCLUDED_
#define _CS_COMMON_H_INCLUDED_


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


/* memory */
void *cs_malloc(size_t s);
void *cs_realloc(void *p, size_t s);
void cs_free(void *p);


/* time */
char *get_time(void);
long long get_ms(void);


/* thread name */
void set_threadname(char *name);
char *get_threadname(void);


#endif /* _CS_COMMON_H_INCLUDED_ */
