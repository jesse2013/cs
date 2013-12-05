/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#ifndef _CS_SQLITE_H_INCLUDED_
#define _CS_SQLITE_H_INCLUDED_


#include <sqlite3.h>

#include "cs_debug.h"


#define QUERY_LEN_MAX   512


typedef struct {
    int         req_type;
    char        *name;
    char        *passwd;
    char        *buddy_name;
    char        *content;
    char        *datetime;
} cs_request_t;

typedef struct {
    sqlite3 *db;
    char    *name;
} destroy_t;

typedef struct {
    char        *email;
    char        *phone;
    char        *tel;
} user_info_t;


int sql_routine(int fd, sqlite3 *db, sockfd_buf_t *rwbuf);


#endif /* _CS_SQLITE_H_INCLUDED_ */
