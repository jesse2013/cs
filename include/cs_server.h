/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#ifndef _CS_SERVER_H_INCLUDED_
#define _CS_SERVER_H_INCLUDED_


#include "cs_debug.h"
#include "cs_sqlite.h"


#define FD_MAX          1024

#define BUF_INCREMENT   1024


typedef struct {
    char            *r_buf;
    int             r_len;
    int             r_len_max;

    char            *w_buf;
    int             w_len;
    int             w_len_max;
} cli_buf_t;

typedef struct {
    int             serv_fd;
    int             listen_num;

    int             cli_fd[FD_MAX];
    cli_buf_t       cli_buf[FD_MAX];

    int             nfds;
    fd_set          readfds;
    fd_set          writefds;
    fd_set          exceptfds;
    struct timeval  timeout;

    sqlite3         *db;
} cs_t;


#endif /* _CS_SERVER_H_INCLUDED_ */
