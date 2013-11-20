/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#ifndef _CS_SERVER_H_INCLUDED_
#define _CS_SERVER_H_INCLUDED_


#include <sqlite3.h>

#include "cs_debug.h"


#define FD_MAX          1024
#define QUERY_LEN_MAX   512


typedef struct {
    int req_type;
    char *name;
    char *passwd;
    char *buddy_name;
    char *content;
    char *datetime;
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
    char *name;
    char *content;
    char *datetime;
} cs_his_t;


#define BUF_INCREMENT   1024

typedef struct {
    char *r_buf;
    int r_len;
    int r_len_max;

    char *w_buf;
    int w_len;
    int w_len_max;
} cli_buf_t;

typedef struct {
    int serv_fd;

    int fd_num;
    int cli_fd[FD_MAX];
    cli_buf_t cli_buf[FD_MAX];
} cs_t;


#endif /* _CS_SERVER_H_INCLUDED_ */
