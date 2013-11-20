/************************************************************
 * Email: troyyuan@smit.com.cn
 * *********************************************************/

#include "cs_server.h"


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

int sql_insert_cb(void *p, int argc, char **value, char **name)
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


char *sql_register(cs_request_t *req)
{
    if (req->name == NULL || req->passwd == NULL) {
        E("parameter error.");
        return NULL;
    }

    /* insert */
    /* create */
    return NULL;
}


int sql_check_identity_cb(void *p, int argc, char **value, char **name)
{
    (*(int *)p)++;
    return 0;
}

int sql_get_buddy_cb(void *p, int argc, char **value, char **name)
{
    char *buf = (char *)p;
    int len = strlen(buf);

    sprintf(buf + len, ":%s-%s", value[1], value[2]);

    return 0;
}

char *sql_login(cs_request_t *req, sqlite3 *db)
{
    if (req->name == NULL || req->passwd == NULL) {
        E("parameter error.");
        return NULL;
    }

    /* check identify */
    char *query_line = (char *)cs_malloc(sizeof(char) * QUERY_LEN_MAX);
    if (query_line == NULL) {
        E("cs_malloc() failed.");
        return NULL;
    }

    sprintf(query_line, "select * from user where name='%s' and passwd='%s'", req->name, req->passwd);
    DS(query_line);

    int sql_select_num = 0;
    int ret = sqlite3_exec(db, query_line, sql_check_identity_cb, &sql_select_num, NULL);
    if (ret == SQLITE_ABORT || sql_select_num != 1) {
        E("sqlite3_exec() failed.");
        DD(sql_select_num);
        /*
        s = write(peer_sockfd, "*", 1);
        if (s == -1) {
            E("%s", strerror(errno));
        }
        D("send sign * to client.");
        continue;
        */
        cs_free(&query_line);
        return NULL;
    }

    /* get buddy name list */
    memset(query_line, '\0', QUERY_LEN_MAX);
    sprintf(query_line, "select * from %s", req->name);
    DS(query_line);

    char *buf = (char *)cs_malloc(sizeof(char) * 512);
    if (buf == NULL) {
        E("cs_malloc() failed.");
        cs_free(&query_line);
        return NULL;
    }

    ret = sqlite3_exec(db, query_line, sql_get_buddy_cb, &buf, NULL);
    if (ret == SQLITE_ABORT) {
        E("sqlite3_exec() failed.");
        cs_free(&buf);
        cs_free(&query_line);
        return NULL;
    }
    DS(buf);

    if (strlen(buf) == 0) {
        /*
        s = write(peer_sockfd, ":", 1);
        if (s == -1) {
            E("%s", strerror(errno));
        
        D("send sign : to client.");
        continue;
        */
    }

    cs_free(&query_line);
    return buf;
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
                req.req_type = atoi(token);
                break;
            case 1:
                req.name = strdup(token);
                break;
            case 2:
                req.passwd = strdup(token);
                break;
            case 3:
                req.buddy_name = strdup(token);
                break;
            case 4:
                req.content = strdup(token);
                break;
            case 5:
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

char *sql_routine(char *buf)
{
    if (buf == NULL) {
        E("parameter error.");
        return NULL;
    }

    /* :0:name:passwd:name:content:datetime */
    char *regex = "^:[0-9]{1}:[A-Za-z0-9_]*:[A-Za-z0-9_]*:"
                    "[A-Za-z0-9_]*:.*:[0-9]{0,14}$";
    if (cs_regex(buf, regex) != 0) {
        E("cs_regex() failed.");
        DS(buf);
        return NULL;
    }

    cs_request_t req = cs_parse_request(buf);
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
            ret = sql_register(&req);
            break;
        case 1:
            /* login - check username & passwd */
            ret = sql_login(&req, db);
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


int main(int argc, char *argv[])
{
    int sockfd = -1;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        E("%s", strerror(errno));
        return -1;
    }

    int optval = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
            &optval, sizeof(optval));
    if (ret == -1) {
        E("%s", strerror(errno));
        return -1;
    }

#ifdef SO_REUSEPORT
    optval = 1;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT,
            &optval, sizeof(optval));
    if (ret == -1) {
        E("%s", strerror(errno));
        return -1;
    }
#endif

    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    memset(&addr, '\0', addrlen);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(sockfd, (struct sockaddr *)&addr, addrlen);
    if (ret == -1) {
        E("%s", strerror(errno));
        return -1;
    }

    int backlog = 20;
    ret = listen(sockfd, backlog);
    if (ret == -1) {
        E("%s", strerror(errno));
        return -1;
    }

    struct sockaddr_in peer_addr;
    socklen_t peer_addrlen = sizeof(peer_addr);

    memset(&peer_addr, '\0', peer_addrlen);

    size_t buflen = 512;
    char *buf = (char *)cs_malloc(sizeof(char) * buflen);
    if (buf == NULL) {
        E("cs_malloc() failed.");
        return -1;
    }

    int peer_sockfd = -1;
    ssize_t s = 0;
    char str[INET_ADDRSTRLEN];
    char *ret_buf = NULL;
    while (1) {
        peer_sockfd = accept(sockfd,
                (struct sockaddr *)&peer_addr, &peer_addrlen);
        if (peer_sockfd == -1) {
            E("%s", strerror(errno));
            break;
        }
        D("received from %s at PORT %d",
                inet_ntop(AF_INET, &peer_addr.sin_addr, str,
                    sizeof(str)),
                ntohs(peer_addr.sin_port));

        // FIXME: why server exit when client ctrl-c
        while (1) {
            memset(buf, '\0', buflen);

            s = read(peer_sockfd, buf, buflen);
            if (s == -1 || strlen(buf) == 0) {
                //E("%s", strerror(errno));
                break;
            }
            DS(buf);

            ret_buf = sql_routine(buf);
            if (ret_buf == NULL) {
                E("sql_routine() failed.");
                break;
            }

            s = write(peer_sockfd, ret_buf, strlen(ret_buf));
            if (s == -1) {
                E("%s", strerror(errno));
                break;
            }

            cs_free(&ret_buf);
        }
    }

    cs_free(&buf);
    ret = close(sockfd);
    if (ret == -1) {
        E("%s", strerror(errno));
        return -1;
    }

#if 0
    void FD_CLR(int fd, fd_set *set);
    int  FD_ISSET(int fd, fd_set *set);
    void FD_SET(int fd, fd_set *set);
    void FD_ZERO(fd_set *set);
    int select(int nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, struct timeval *timeout);
#endif

    return 0;
}
