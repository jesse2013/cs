/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#include "cs_sqlite.h"


/* user register */
int sql_check_name_cb(void *p, int argc, char **value, char **name)
{
    (*(int *)p)++;
    return 0;
}

int sql_register(cs_request_t *req, sqlite3 *db, buf_t *wbuf)
{
    if (req == NULL || req->name == NULL || req->passwd == NULL ||
        db == NULL || wbuf == NULL || wbuf->data == NULL) {
        E("parameter error.");
        return -1;
    }

    char *query_line = (char *)cs_malloc(sizeof(char) * QUERY_LEN_MAX);
    if (query_line == NULL) {
        E("cs_malloc() failed.");
        return -1;
    }

    /* search user name whether exist */
    sprintf(query_line, "select * from users where name='%s'", req->name);
    DS(query_line);

    int sql_select_num = 0;
    int ret = sqlite3_exec(db, query_line, sql_check_name_cb, &sql_select_num, NULL);
    if (ret == SQLITE_OK && sql_select_num == 1) {
        D(GREEN"user name %s is exist."NO, req->name);
        cs_free(&query_line);

        strncpy(wbuf->data, "12", 2);
        wbuf->len = 2;
        return 0;
    }

    /* insert user info to users table */
    memset(query_line, '\0', QUERY_LEN_MAX);
    sprintf(query_line, "insert into users(name, passwd, fd) values('%s', '%s', -1)", req->name, req->passwd);
    DS(query_line);

    ret = sqlite3_exec(db, query_line, NULL, NULL, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }

    /* create private tables */
    memset(query_line, '\0', QUERY_LEN_MAX);
    sprintf(query_line, "create table %s(id integer primary key, name text, log_type integer)", req->name);
    DS(query_line);

    ret = sqlite3_exec(db, query_line, NULL, NULL, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }

    cs_free(&query_line);

    strncpy(wbuf->data, "10", 2);
    wbuf->len = 2;

    D(GREEN"register user %s success.", req->name);
    return 0;
}


/* user destroy */
int sql_destroy(cs_request_t *req, sqlite3 *db, buf_t *wbuf)
{
    /* check identify */
    /* delete user */
    strncpy(wbuf->data, "00", 2);
    wbuf->len = 2;
    return 0;
}


/* login */
int sql_check_identity_cb(void *p, int argc, char **value, char **name)
{
    (*(int *)p)++;
    return 0;
}

int sql_get_buddy_cb(void *p, int argc, char **value, char **name)
{
    buf_t *wbuf = (buf_t *)p;
    sprintf(wbuf->data + wbuf->len, ":%s-%s", value[0], value[1]);
    wbuf->len = strlen(wbuf->data);
    return 0;
}

int sql_login(int fd, cs_request_t *req, sqlite3 *db, buf_t *wbuf)
{
    if (req == NULL || req->name == NULL || req->passwd == NULL ||
        db == NULL || wbuf == NULL || wbuf->data == NULL) {
        E("parameter error.");
        return -1;
    }

    /* check identify */
    char *query_line = (char *)cs_malloc(sizeof(char) * QUERY_LEN_MAX);
    if (query_line == NULL) {
        E("cs_malloc() failed.");
        DPSTR(wbuf);
        return -1;
    }

    sprintf(query_line, "select * from users where name='%s' and passwd='%s'", 
            req->name, req->passwd);
    DS(query_line);

    int sql_select_num = 0;
    int ret = sqlite3_exec(db, query_line, sql_check_identity_cb, 
                &sql_select_num, NULL);
    if (ret == SQLITE_ABORT || sql_select_num != 1) {
        /* no this user & passwd */
        E("sqlite3_exec() failed.");
        DD(sql_select_num);
        cs_free(&query_line);

        strncpy(wbuf->data, "*", 1);
        wbuf->len = 1;
        return -1;
    }

    /* update user info in users table */
    memset(query_line, '\0', QUERY_LEN_MAX);
    sprintf(query_line, "update users set fd=%d where name='%s'", fd, req->name);
    DS(query_line);

    ret = sqlite3_exec(db, query_line, NULL, NULL, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }

    /* get buddy name list */
    memset(query_line, '\0', QUERY_LEN_MAX);
    sprintf(query_line, "select %s.name, users.fd from %s,users where %s.name=users.name;", req->name, req->name, req->name);
    DS(query_line);

    ret = sqlite3_exec(db, query_line, sql_get_buddy_cb, wbuf, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }
    //DPSTR(wbuf);

    /* user haven't buddy */
    if (wbuf->len == 0) {
        strncpy(wbuf->data, ":", 1);
        wbuf->len = 1;

        cs_free(&query_line);
        return 0;
    }

    cs_free(&query_line);

    D(GREEN"user %s login success.", req->name);
    return 0;
}


/* logout */
int sql_logout(cs_request_t *req, sqlite3 *db, buf_t *wbuf)
{
    strncpy(wbuf->data, "00", 2);
    wbuf->len = 2;
    return 0;
}


/* view all users */
int sql_view_user_cb(void *p, int argc, char **value, char **name)
{
    buf_t *wbuf = (buf_t *)p;
    sprintf(wbuf->data + wbuf->len, ":%s-%s", value[1], value[3]);
    wbuf->len = strlen(wbuf->data);
    return 0;
}

int sql_view_user(cs_request_t *req, sqlite3 *db, buf_t *wbuf)
{
    if (req == NULL || db == NULL || wbuf == NULL || 
        wbuf->data == NULL) {
        E("parameter error.");
        return -1;
    }

    /* check identify */
    char *query_line = (char *)cs_malloc(sizeof(char) * QUERY_LEN_MAX);
    if (query_line == NULL) {
        E("cs_malloc() failed.");
        DPSTR(wbuf);
        return -1;
    }

    sprintf(query_line, "select * from users");
    DS(query_line);

    int ret = sqlite3_exec(db, query_line, sql_view_user_cb, wbuf, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }
    //DPSTR(wbuf);

    /* user haven't buddy */
    if (wbuf->len == 0) {
        strncpy(wbuf->data, ":", 1);
        wbuf->len = 1;

        cs_free(&query_line);
        D(YELLOW"nothing in users table."NO);
        return 0;
    }

    cs_free(&query_line);

    D(GREEN"view all users success."NO);
    return 0;
}


/* add buddy */
int sql_check_relationship_cb(void *p, int argc, char **value, char **name)
{
    (*(int *)p)++;
    return 0;
}

int sql_add_buddy(cs_request_t *req, sqlite3 *db, buf_t *wbuf)
{
    if (req == NULL || req->name == NULL || req->buddy_name == NULL ||
        db == NULL || wbuf == NULL || wbuf->data == NULL) {
        E("parameter error.");
        return -1;
    }

    /* check relationship */
    char *query_line = (char *)cs_malloc(sizeof(char) * QUERY_LEN_MAX);
    if (query_line == NULL) {
        E("cs_malloc() failed.");
        DPSTR(wbuf);
        return -1;
    }

    sprintf(query_line, "select * from %s where name='%s'", req->name, req->buddy_name);
    DS(query_line);

    int sql_select_num = 0;
    int ret = sqlite3_exec(db, query_line, sql_check_relationship_cb, &sql_select_num, NULL);
    if (ret == SQLITE_OK && sql_select_num == 1) {
        D(GREEN"%s and %s are buddy."NO, req->buddy_name, req->name);
        cs_free(&query_line);

        strncpy(wbuf->data, "12", 2);
        wbuf->len = 2;
        return 0;
    }

    /* update mine table */
    memset(query_line, '\0', QUERY_LEN_MAX);
    sprintf(query_line, "insert into %s(name, log_type) values('%s', 0)", req->name, req->buddy_name);
    DS(query_line);

    ret = sqlite3_exec(db, query_line, NULL, NULL, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }

    /* update mine table */
    memset(query_line, '\0', QUERY_LEN_MAX);
    sprintf(query_line, "insert into %s(name, log_type) values('%s', 1)", req->buddy_name, req->name);
    DS(query_line);

    ret = sqlite3_exec(db, query_line, NULL, NULL, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }

    /* create log table */
    memset(query_line, '\0', QUERY_LEN_MAX);
    sprintf(query_line, "create table %s_%s(id integer primary key, name text, content text, datetime text)", 
            req->name, req->buddy_name);
    DS(query_line);

    ret = sqlite3_exec(db, query_line, NULL, NULL, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }

    cs_free(&query_line);

    strncpy(wbuf->data, "11", 2);
    wbuf->len = 2;

    D(GREEN"%s add buddy %s success.", req->name, req->buddy_name);
    return 0;
}


/* del buddy */
int sql_log_type_cb(void *p, int argc, char **value, char **name)
{
    *(int *)p = *value[2];
    return 0;
}

int sql_del_buddy(cs_request_t *req, sqlite3 *db, buf_t *wbuf)
{
    if (req == NULL || req->name == NULL || req->buddy_name == NULL ||
        db == NULL || wbuf == NULL || wbuf->data == NULL) {
        E("parameter error.");
        return -1;
    }

    /* check relationship */
    char *query_line = (char *)cs_malloc(sizeof(char) * QUERY_LEN_MAX);
    if (query_line == NULL) {
        E("cs_malloc() failed.");
        DPSTR(wbuf);
        return -1;
    }

    sprintf(query_line, "select * from %s where name='%s'", req->name, req->buddy_name);
    DS(query_line);

    int sql_select_num = 0;
    int ret = sqlite3_exec(db, query_line, sql_check_relationship_cb, &sql_select_num, NULL);
    if (ret == SQLITE_OK && sql_select_num == 0) {
        D(GREEN"%s and %s isn't buddy."NO, req->buddy_name, req->name);
        cs_free(&query_line);

        strncpy(wbuf->data, "12", 2);
        wbuf->len = 2;
        return 0;
    }

    /* update mine table */
    memset(query_line, '\0', QUERY_LEN_MAX);
    sprintf(query_line, "delete from %s where name='%s'", req->name, req->buddy_name);
    DS(query_line);

    ret = sqlite3_exec(db, query_line, NULL, NULL, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }

    /* update mine table */
    memset(query_line, '\0', QUERY_LEN_MAX);
    sprintf(query_line, "delete from %s where name='%s'", req->buddy_name, req->name);
    DS(query_line);

    ret = sqlite3_exec(db, query_line, NULL, NULL, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }

    /* check log_type */
    memset(query_line, '\0', QUERY_LEN_MAX);
    sprintf(query_line, "select * from %s where name='%s'", req->name, req->buddy_name);
    DS(query_line);

    int log_type = -1;
    ret = sqlite3_exec(db, query_line, sql_log_type_cb, &log_type, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }

    /* delete log table */
    memset(query_line, '\0', QUERY_LEN_MAX);

    if (log_type == 0)
        sprintf(query_line, "drop table %s_%s", req->name, req->buddy_name);
    else if (log_type == 1)
        sprintf(query_line, "drop table %s_%s", req->buddy_name, req->name);
    else
        /* undefined */

    DS(query_line);

    ret = sqlite3_exec(db, query_line, NULL, NULL, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }

    cs_free(&query_line);

    strncpy(wbuf->data, "11", 2);
    wbuf->len = 2;

    D(GREEN"%s delete buddy %s success.", req->name, req->buddy_name);
    return 0;
}


/* chat with buddy */
int sql_sendto(cs_request_t *req, sqlite3 *db, buf_t *wbuf)
{
    if (req == NULL || req->name == NULL || req->buddy_name == NULL ||
        db == NULL || wbuf == NULL || wbuf->data == NULL) {
        E("parameter error.");
        return -1;
    }

    /* check relationship */
    char *query_line = (char *)cs_malloc(sizeof(char) * QUERY_LEN_MAX);
    if (query_line == NULL) {
        E("cs_malloc() failed.");
        DPSTR(wbuf);
        return -1;
    }

    sprintf(query_line, "select * from %s where name='%s'", req->name, req->buddy_name);
    DS(query_line);

    int sql_select_num = 0;
    int ret = sqlite3_exec(db, query_line, sql_check_relationship_cb, &sql_select_num, NULL);
    if (ret == SQLITE_OK && sql_select_num == 0) {
        D(GREEN"%s and %s isn't buddy."NO, req->buddy_name, req->name);
        cs_free(&query_line);

        strncpy(wbuf->data, "12", 2);
        wbuf->len = 2;
        return 0;
    }

    /* check log_type */
    memset(query_line, '\0', QUERY_LEN_MAX);
    sprintf(query_line, "select * from %s where name='%s'", req->name, req->buddy_name);
    DS(query_line);

    int log_type = -1;
    ret = sqlite3_exec(db, query_line, sql_log_type_cb, &log_type, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }

    /* update log table */
    memset(query_line, '\0', QUERY_LEN_MAX);

    if (log_type == 0)
        sprintf(query_line, "insert into %s_%s(name, content, datetime) values('%s', '%s', '%s')", 
                req->name, req->buddy_name, req->name, req->content, req->datetime);
    else if (log_type == 1)
        sprintf(query_line, "insert into %s_%s(name, content, datetime) values('%s', '%s', '%s')", 
                req->buddy_name, req->name, req->name, req->content, req->datetime);
    else
        /* undefined */

    DS(query_line);

    ret = sqlite3_exec(db, query_line, NULL, NULL, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }

    cs_free(&query_line);

    wbuf->len = strlen(req->content);
    strncpy(wbuf->data, req->content, wbuf->len);

    D(GREEN"%s send %s to  %s.", req->name, req->content, req->buddy_name);
    return 0;
}


/* view log */
int sql_get_log_cb(void *p, int argc, char **value, char **name)
{
    buf_t *wbuf = (buf_t *)p;
    sprintf(wbuf->data + wbuf->len, ":%s-%s-%s", value[1], value[3], value[2]);
    wbuf->len = strlen(wbuf->data);
    return 0;
}

int sql_view_log(cs_request_t *req, sqlite3 *db, buf_t *wbuf)
{
    if (req == NULL || req->name == NULL || req->buddy_name == NULL || 
        db == NULL || wbuf == NULL || wbuf->data == NULL) {
        E("parameter error.");
        return -1;
    }

    /* check log_type */
    char *query_line = (char *)cs_malloc(sizeof(char) * QUERY_LEN_MAX);
    if (query_line == NULL) {
        E("cs_malloc() failed.");
        DPSTR(wbuf);
        return -1;
    }

    sprintf(query_line, "select * from %s where name='%s'", req->name, req->buddy_name);
    DS(query_line);

    int log_type = -1;
    int ret = sqlite3_exec(db, query_line, sql_log_type_cb, &log_type, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }
    
    /* get log content */
    memset(query_line, '\0', QUERY_LEN_MAX);

    if (log_type == 0)
        sprintf(query_line, "select * from %s-%s", req->name, req->buddy_name);
    else if (log_type == 1)
        sprintf(query_line, "select * from %s-%s", req->buddy_name, req->name);
    else
        /* undefined */

    DS(query_line);

    ret = sqlite3_exec(db, query_line, sql_get_log_cb, wbuf, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }

    cs_free(&query_line);

    D(GREEN"view log %s-%s success.", req->name, req->buddy_name);
    return 0;
}


/* del log */
int sql_del_log(cs_request_t *req, sqlite3 *db, buf_t *wbuf)
{
    strncpy(wbuf->data, "00", 2);
    wbuf->len = 2;
    return 0;
}


int sql_routine(int fd, sqlite3 *db, cs_request_t *req, sockfd_buf_t *rwbuf)
{
    if (fd < 0 || db == NULL || req == NULL || req->req_type < 0|| 
        rwbuf == NULL || rwbuf->rbuf.data == NULL || 
        rwbuf->rbuf.len == 0 || rwbuf->wbuf.data == NULL) {
        E("parameter error.");
        DD(fd);
        DP(db);
        DP(req);
        DP(rwbuf);
        DD(req->req_type);
        DP(rwbuf);
        DP(rwbuf->rbuf.data);
        DD(rwbuf->rbuf.len);
        DP(rwbuf->wbuf.data);
        DD(rwbuf->wbuf.len);
        return -1;
    }

    int ret = -1;
    switch (req->req_type) {
        case 0:
            ret = sql_register(req, db, &rwbuf->wbuf);
            break;
        case 1:
            ret = sql_destroy(req, db, &rwbuf->wbuf);
            break;
        case 2:
            ret = sql_login(fd, req, db, &rwbuf->wbuf);
            break;
        case 3:
            ret = sql_logout(req, db, &rwbuf->wbuf);
            break;
        case 4:
            ret = sql_view_user(req, db, &rwbuf->wbuf);
            break;
        case 5:
            ret = sql_add_buddy(req, db, &rwbuf->wbuf);
            break;
        case 6:
            ret = sql_del_buddy(req, db, &rwbuf->wbuf);
            break;
        case 7:
            ret = sql_sendto(req, db, &rwbuf->wbuf);
            break;
        case 8:
            ret = sql_view_log(req, db, &rwbuf->wbuf);
            break;
        case 9:
            ret = sql_del_log(req, db, &rwbuf->wbuf);
            break;
        default:
            DD(req->req_type);
            break;
    }

    return ret; 
}



/* use buddy name get buddy fd */
int sql_buddy_fd_cb(void *p, int argc, char **value, char **name)
{
    *(int *)p = atoi(value[3]);
    return 0;
}

int sql_find_buddy_fd(cs_request_t *req, sqlite3 *db)
{
    if (req == NULL || req->name == NULL || 
        req->buddy_name == NULL || db == NULL) {
        E("parameter error.");
        return -1;
    }

    char *query_line = (char *)cs_malloc(sizeof(char) * QUERY_LEN_MAX);
    if (query_line == NULL) {
        E("cs_malloc() failed.");
        return -1;
    }

    sprintf(query_line, "select * from users where name='%s'", 
            req->buddy_name);
    DS(query_line);

    int fd = 0;
    int ret = sqlite3_exec(db, query_line, sql_buddy_fd_cb, &fd, NULL);
    if (ret == SQLITE_ABORT || fd < 0) {
        E("sqlite3_exec() failed.");
        DD(fd);
        cs_free(&query_line);
        return -1;
    }

    cs_free(&query_line);

    D(GREEN"user %s fd is %d.", req->buddy_name, fd);
    return fd;
}
