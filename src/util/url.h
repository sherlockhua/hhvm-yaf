/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-07 00:08:05
*   description  ：
*
=============================================*/
#ifndef _URL_H_
#define _URL_H_

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define STR_FREE(ptr) if (ptr) { free(ptr); }  

typedef struct php_url {
    char *scheme;
    char *user;
    char *pass;
    char *host;
    unsigned short port;
    char *path;
    char *query;
    char *fragment;
} php_url;


extern php_url *php_url_parse(char const *str);
extern php_url *php_url_parse_ex(char const *str, int length);
extern char *php_replace_controlchars_ex(char *str, int len);
extern void php_url_free(php_url *theurl);

#endif //_URL_H
