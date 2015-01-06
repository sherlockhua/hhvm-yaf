#include "url.h"


char *php_replace_controlchars_ex(char *str, int len)
{
    unsigned char *s = (unsigned char *)str;
    unsigned char *e = (unsigned char *)str + len;
    
    if (!str) {
        return (NULL);
    }
    
    while (s < e) {
        if (iscntrl(*s)) {
            *s='_';
        }   
        s++;
    }
    
    return (str);
}

php_url *php_url_parse(char const *str)
{
    return php_url_parse_ex(str, strlen(str));
}

php_url *php_url_parse_ex(char const *str, int length)
{
	char port_buf[6];
	php_url *ret = (php_url*)calloc(1, sizeof(php_url));
	char const *s, *e, *p, *pp, *ue;
		
	s = str;
	ue = s + length;

	/* parse scheme */
	if ((e = (char*)memchr(s, ':', length)) && (e - s)) {
		/* validate scheme */
		p = s;
		while (p < e) {
			/* scheme = 1*[ lowalpha | digit | "+" | "-" | "." ] */
			if (!isalpha(*p) && !isdigit(*p) && *p != '+' && *p != '.' && *p != '-') {
				if (e + 1 < ue) {
					goto parse_port;
				} else {
					goto just_path;
				}
			}
			p++;
		}
	
		if (*(e + 1) == '\0') { /* only scheme is available */
			ret->scheme = strndup(s, (e - s));
			php_replace_controlchars_ex(ret->scheme, (e - s));
			goto end;
		}

		/* 
		 * certain schemas like mailto: and zlib: may not have any / after them
		 * this check ensures we support those.
		 */
		if (*(e+1) != '/') {
			/* check if the data we get is a port this allows us to 
			 * correctly parse things like a.com:80
			 */
			p = e + 1;
			while (isdigit(*p)) {
				p++;
			}
			
			if ((*p == '\0' || *p == '/') && (p - e) < 7) {
				goto parse_port;
			}
			
			ret->scheme = strndup(s, (e-s));
			php_replace_controlchars_ex(ret->scheme, (e - s));
			
			length -= ++e - s;
			s = e;
			goto just_path;
		} else {
			ret->scheme = strndup(s, (e-s));
			php_replace_controlchars_ex(ret->scheme, (e - s));
		
			if (*(e+2) == '/') {
				s = e + 3;
				if (!strncasecmp("file", ret->scheme, sizeof("file"))) {
					if (*(e + 3) == '/') {
						/* support windows drive letters as in:
						   file:///c:/somedir/file.txt
						*/
						if (*(e + 5) == ':') {
							s = e + 4;
						}
						goto nohost;
					}
				}
			} else {
				if (!strncasecmp("file", ret->scheme, sizeof("file"))) {
					s = e + 1;
					goto nohost;
				} else {
					length -= ++e - s;
					s = e;
					goto just_path;
				}	
			}
		}	
	} else if (e) { /* no scheme; starts with colon: look for port */
		parse_port:
		p = e + 1;
		pp = p;

		while (pp-p < 6 && isdigit(*pp)) {
			pp++;
		}

		if (pp - p > 0 && pp - p < 6 && (*pp == '/' || *pp == '\0')) {
			long port;
			memcpy(port_buf, p, (pp - p));
			port_buf[pp - p] = '\0';
			port = strtol(port_buf, NULL, 10);
			if (port > 0 && port <= 65535) {
				ret->port = (unsigned short) port;
			} else {
				STR_FREE(ret->scheme);
				free(ret);
				return NULL;
			}
		} else if (p == pp && *pp == '\0') {
			STR_FREE(ret->scheme);
			free(ret);
			return NULL;
		} else {
			goto just_path;
		}
	} else {
		just_path:
		ue = s + length;
		goto nohost;
	}
	
	e = ue;
	
	if (!(p = (char*)memchr(s, '/', (ue - s)))) {
		char *query, *fragment;

		query = (char*)memchr(s, '?', (ue - s));
		fragment = (char*)memchr(s, '#', (ue - s));

		if (query && fragment) {
			if (query > fragment) {
				p = e = fragment;
			} else {
				p = e = query;
			}
		} else if (query) {
			p = e = query;
		} else if (fragment) {
			p = e = fragment;
		}
	} else {
		e = p;
	}	
		
	/* check for login and password */
	if ((p = (char*)memrchr(s, '@', (e-s)))) {
		if ((pp = (char*)memchr(s, ':', (p-s)))) {
			if ((pp-s) > 0) {
				ret->user = strndup(s, (pp-s));
				php_replace_controlchars_ex(ret->user, (pp - s));
			}	
		
			pp++;
			if (p-pp > 0) {
				ret->pass = strndup(pp, (p-pp));
				php_replace_controlchars_ex(ret->pass, (p-pp));
			}	
		} else {
			ret->user = strndup(s, (p-s));
			php_replace_controlchars_ex(ret->user, (p-s));
		}
		
		s = p + 1;
	}

	/* check for port */
	if (*s == '[' && *(e-1) == ']') {
		/* Short circuit portscan, 
		   we're dealing with an 
		   IPv6 embedded address */
		p = s;
	} else {
		/* memrchr is a GNU specific extension
		   Emulate for wide compatability */
		for(p = e; *p != ':' && p >= s; p--);
	}

	if (p >= s && *p == ':') {
		if (!ret->port) {
			p++;
			if (e-p > 5) { /* port cannot be longer then 5 characters */
				STR_FREE(ret->scheme);
				STR_FREE(ret->user);
				STR_FREE(ret->pass);
				free(ret);
				return NULL;
			} else if (e - p > 0) {
				long port;
				memcpy(port_buf, p, (e - p));
				port_buf[e - p] = '\0';
				port = strtol(port_buf, NULL, 10);
				if (port > 0 && port <= 65535) {
					ret->port = (unsigned short)port;
				} else {
					STR_FREE(ret->scheme);
					STR_FREE(ret->user);
					STR_FREE(ret->pass);
					free(ret);
					return NULL;
				}
			}
			p--;
		}	
	} else {
		p = e;
	}
	
	/* check if we have a valid host, if we don't reject the string as url */
	if ((p-s) < 1) {
		STR_FREE(ret->scheme);
		STR_FREE(ret->user);
		STR_FREE(ret->pass);
		free(ret);
		return NULL;
	}

	ret->host = strndup(s, (p-s));
	php_replace_controlchars_ex(ret->host, (p - s));
	
	if (e == ue) {
		return ret;
	}
	
	s = e;
	
	nohost:
	
	if ((p = (char*)memchr(s, '?', (ue - s)))) {
		pp = strchr(s, '#');

		if (pp && pp < p) {
			if (pp - s) {
				ret->path = strndup(s, (pp-s));
				php_replace_controlchars_ex(ret->path, (pp - s));
			}
			p = pp;
			goto label_parse;
		}
	
		if (p - s) {
			ret->path = strndup(s, (p-s));
			php_replace_controlchars_ex(ret->path, (p - s));
		}	
	
		if (pp) {
			if (pp - ++p) { 
				ret->query = strndup(p, (pp-p));
				php_replace_controlchars_ex(ret->query, (pp - p));
			}
			p = pp;
			goto label_parse;
		} else if (++p - ue) {
			ret->query = strndup(p, (ue-p));
			php_replace_controlchars_ex(ret->query, (ue - p));
		}
	} else if ((p = (char*)memchr(s, '#', (ue - s)))) {
		if (p - s) {
			ret->path = strndup(s, (p-s));
			php_replace_controlchars_ex(ret->path, (p - s));
		}	
		
		label_parse:
		p++;
		
		if (ue - p) {
			ret->fragment = strndup(p, (ue-p));
			php_replace_controlchars_ex(ret->fragment, (ue - p));
		}	
	} else {
		ret->path = strndup(s, (ue-s));
		php_replace_controlchars_ex(ret->path, (ue - s));
	}
end:
	return ret;
}
