/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#ifndef _CS_SERVER_H_INCLUDED_
#define _CS_SERVER_H_INCLUDED_


#include <sqlite3.h>

#include "cs_debug.h"

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
    int fd_max;
    int serv_fd;
    int cli_fd_list;
    cli_buf_t cli_buf;
} cs_t;


#endif /* _CS_SERVER_H_INCLUDED_ */
