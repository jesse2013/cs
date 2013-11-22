/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#include "cs_sqlite.h"


void request_init(cs_request_t *req)
{
    req->req_type = -1;
    req->name = NULL;
    req->passwd = NULL;
    req->buddy_name = NULL;
    req->content = NULL;
    req->datetime = NULL;
}

void request_dump(cs_request_t *req)
{
    D("***********************************");
    DD(req->req_type);
    DSIF(req->name);
    DSIF(req->passwd);
    DSIF(req->buddy_name);
    DSIF(req->content);
    DSIF(req->datetime);
    D("***********************************");
}

void request_free(cs_request_t *req)
{
    cs_free(&req->name);
    cs_free(&req->passwd);
    cs_free(&req->buddy_name);
    cs_free(&req->content);
    cs_free(&req->datetime);
}


int sql_create_cb(void *p, int argc, char **value, char **name)
{
    return 0;
}

int sql_insert_cb()
{
    return 0;
}

int sql_select_cb(void *p, int argc, char **value, char **name)
{
    return 0;
}

int sql_update_cb(void *p, int argc, char **value, char **name)
{
    return 0;
}

int sql_delete_cb(void *p, int argc, char **value, char **name)
{
    return 0;
}


int sql_register(cs_request_t *req, sqlite3 *db)
{
    if (req->name == NULL || req->passwd == NULL) {
        E("parameter error.");
        return -1;
    }

    /* insert user info to user tables */
    char *query_line = (char *)cs_malloc(sizeof(char) * QUERY_LEN_MAX);
    if (query_line == NULL) {
        E("cs_malloc() failed.");
        return -1;
    }

    sprintf(query_line, "insert into user(id, name, passwd) values(10, '%s', '%s')", req->name, req->passwd);
    DS(query_line);

    int ret = sqlite3_exec(db, query_line, NULL, NULL, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }

    /* create private tables */
    memset(query_line, '\0', QUERY_LEN_MAX);
    sprintf(query_line, "create table %s(id integer primary key, name text, online text)" , req->name);
    DS(query_line);

    ret = sqlite3_exec(db, query_line, NULL, NULL, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return -1;
    }

    cs_free(&query_line);
    return 0;
}


int sql_check_identity_cb(void *p, int argc, char **value, char **name)
{
    (*(int *)p)++;
    return 0;
}

int sql_get_buddy_cb(void *p, int argc, char **value, char **name)
{
    buf_t *wbuf = (buf_t *)p;
    sprintf(wbuf->data + wbuf->len, ":%s-%s", value[1], value[2]);
    wbuf->len = strlen(wbuf->data);
    return 0;
}

char *sql_login(cs_request_t *req, sqlite3 *db, buf_t *wbuf)
{
    if (req->name == NULL || req->passwd == NULL || db == NULL || 
        wbuf == NULL || wbuf->data == NULL) {
        E("parameter error.");
        return NULL;
    }

    /* check identify */
    char *query_line = (char *)cs_malloc(sizeof(char) * QUERY_LEN_MAX);
    if (query_line == NULL) {
        E("cs_malloc() failed.");
        DPSTR(wbuf);
        return NULL;
    }

    sprintf(query_line, "select * from user where name='%s' and passwd='%s'", 
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
        return NULL;
    }

    /* get buddy name list */
    memset(query_line, '\0', QUERY_LEN_MAX);
    sprintf(query_line, "select * from %s", req->name);
    DS(query_line);

    ret = sqlite3_exec(db, query_line, sql_get_buddy_cb, wbuf, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&query_line);
        return NULL;
    }
    DPSTR(wbuf);

    /* user haven't buddy */
    if (wbuf->len == 0) {
        strncpy(wbuf->data, ":", 1);
        wbuf->len = 1;
    }

    cs_free(&query_line);
    return NULL;
}


char *sql_view_user(cs_request_t *req)
{
    /* return all user */
    return NULL;
}


char *sql_sendto(cs_request_t *req)
{
    /* send content to ivy */
    /* insert my log with ivy */
    /* insert ivy log with me */
    return NULL;
}


char *sql_view_log(cs_request_t *req)
{
    /* return chat log with ivy */
    return NULL;
}


char *sql_delete(cs_request_t *req)
{
    /* check identify */
    /* delete user */
    return NULL;
}


int cs_regex(const char *str, const char *regex)
{
    if (str == NULL || regex == NULL) { 
        E("parameter error."); 
        return -1;  
    }    

    regex_t preg;
    int ret = -1;
    ret = regcomp(&preg, regex, REG_EXTENDED);
    if (ret != 0) { 
        E("regcomp() failed."); 
        return -1;  
    }    

    size_t nmatch = 2; 
    regmatch_t pmatch[2];
    ret = regexec(&preg, str, nmatch, pmatch, 0);
    if (ret != 0) { 
        E("regexec() failed."); 
        regfree(&preg); 
        return -1;  
    }    

    regfree(&preg);
    return ret; 
}

cs_request_t cs_parse_request(char *buf)
{
    cs_request_t req;
    request_init(&req);

    if (buf == NULL) {
        E("parameter error.");
        return req;
    }

    char *buf_copy = strdup(buf);
    if (buf_copy == NULL) {
        E("strncup() failed.");
        return req;
    }

    char *str = buf_copy;
    char *token = NULL;
    char *saveptr = NULL;
    int i = 0;
    while (1) {
        token = strtok_r(str, ":", &saveptr);
        if (token == NULL)
            break;

        switch (i) {
            case 0:
                /* req_type */
                req.req_type = atoi(token);
                break;
            case 1:
                /* user name */
                req.name = strdup(token);
                break;
            case 2:
                /* user passwd */
                req.passwd = strdup(token);
                break;
            case 3:
                /* buddy name */
                req.buddy_name = strdup(token);
                break;
            case 4:
                /* content */
                req.content = strdup(token);
                break;
            case 5:
                /* datatime */
                req.datetime = strdup(token);
                break;
            default:
                DD(i);
                break;
        }

        str = NULL;
        i++;
    }
    //request_dump(&req);

    cs_free(&buf_copy);
    return req;
}

char *sql_routine(sockfd_buf_t *rwbuf)
{
    if (rwbuf == NULL || rwbuf->rbuf.data == NULL || rwbuf->rbuf.len == 0 || 
        rwbuf->wbuf.data == NULL) {
        E("parameter error.");
        DP(rwbuf);
        DP(rwbuf->rbuf.data);
        DD(rwbuf->rbuf.len);
        DP(rwbuf->wbuf.data);
        return NULL;
    }

    /* :0:name:passwd:name:content:datetime */
    char *regex = "^:[0-9]{1}:[A-Za-z0-9_]*:[A-Za-z0-9_]*:"
                    "[A-Za-z0-9_]*:.*:[0-9]{0,14}$";
    if (cs_regex(rwbuf->rbuf.data, regex) != 0) {
        E("cs_regex() failed.");
        DDSTR(rwbuf->rbuf);
        return NULL;
    }

    cs_request_t req = cs_parse_request(rwbuf->rbuf.data);
    request_dump(&req);

    sqlite3 *db;
    if (sqlite3_open("./cs_user.db", &db) != SQLITE_OK) {
        E("sqlite3_open() failed.");
        return NULL;
    }

    char *ret = NULL; 
    switch (req.req_type) {
        case 0:
            /* register */
            sql_register(&req, db);
            break;
        case 1:
            /* login - check username & passwd */
            ret = sql_login(&req, db, &rwbuf->wbuf);
            break;
        case 2:
            /* view all user */
            ret = sql_view_user(&req);
            break;
        case 3:
            /* send content to ivy */
            ret = sql_sendto(&req);
            break;
        case 4:
            /* view log with ivy */
            ret = sql_view_log(&req);
            break;
        case 5:
            /* delete user */
            ret = sql_delete(&req);
            break;
        default:
            DD(req.req_type);
            break;
    }

    sqlite3_close(db);
    return ret; 
}
