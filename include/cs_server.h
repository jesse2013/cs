/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#ifndef _CS_SERVER_H_INCLUDED_
#define _CS_SERVER_H_INCLUDED_


#include "cs_debug.h"
#include "cs_sqlite.h"


#define BUF_MAX         1024
typedef struct {
    char            *data;
    int             len;
    int             max;
} buf_t;

typedef struct {
    buf_t           rbuf;
    buf_t           wbuf;
} sockfd_buf_t;


#endif /* _CS_SERVER_H_INCLUDED_ */
