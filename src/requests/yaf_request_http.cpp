/*===========================================
*   Copyright (C) 2014 All rights reserved.
*   
*   company      : xiaomi
*   author       : root
*   email        : root@xiaomi.com
*   date         ：2014-12-18 07:29:18
*   description  ：
*
=============================================*/
#include "yaf_request_http.h"
#include "ext_yaf.h"
#include "hphp/runtime/base/base-includes.h"
#include "yaf_request.h"
#include "url.h"
#include "hphp/runtime/base/php-globals.h"

namespace HPHP{


Variant yaf_request_http_instance(const Object& object, 
        const Variant& request_uri, const Variant& var_base_uri)
{
    Object o;
    if (object.isNull()) {
        Array arr = Array::Create();
        arr.append(request_uri);
        arr.append(var_base_uri);

        o = createObject("Yaf_Request_Http", arr) ;
    } else {
        o = object;
    }

    const char* base_uri = NULL;
    if (var_base_uri.isString() && var_base_uri.toString().length()) {
        base_uri = var_base_uri.toString().c_str();
    }

    auto tmp = o->o_realProp(YAF_REQUEST_PROPERTY_NAME_METHOD, 
        ObjectData::RealPropUnchecked, "Yaf_Request_Http");

    if (php_global(S_SERVER).toArray().exists(String("REQUEST_METHOD"))) {
        Variant request_method = php_global(S_SERVER).toArray()[String("REQUEST_METHOD")];
        *tmp = request_method;
    } else if (strncmp(RuntimeOption::ExecutionMode, "cli", 3) == 0) {
        *tmp = String("Cli");
    } else {
        *tmp = String("Unknown");
    }

    std::string uri;
    if (request_uri.isString()) {
        uri = request_uri.toString().toCppString();
    } else {
        Variant tmp;
        if (php_global(S_SERVER).toArray().exists(String("PATH_INFO"))) {
            uri = php_global(S_SERVER).toArray()[String("PATH_INFO")].toString().toCppString();
            goto done;
        }

        if (php_global(S_SERVER).toArray().exists(String("REQUEST_URI"))) {
            tmp = php_global(S_SERVER).toArray()[String("REQUEST_URI")];
            if (tmp.isString()) {
                std::string str_tmp = tmp.toString().toCppString();
                if (strncasecmp(str_tmp.c_str(), "http", 4) == 0) {
                    //TODO use php_url_parse to get path
                    php_url* url_info = php_url_parse(str_tmp.c_str());
                    if (url_info && url_info->path) {
                        uri = std::string(url_info->path);
                    }
                    php_url_free(url_info);
                } else {
                    const char* tmp_str_tmp = str_tmp.c_str();
                    const char* pos = strstr(tmp_str_tmp, "?");
                    if (pos) {
                        uri = std::string(str_tmp.c_str(), pos - tmp_str_tmp);
                    } else {
                        uri = str_tmp;
                    }
                }
            }

            goto done;
        }

        if (php_global(S_SERVER).toArray().exists(String("ORIG_PATH_INFO"))) {
            uri = php_global(S_SERVER).toArray()[String("ORIG_PATH_INFO")].toString().toCppString();
            goto done;
        } 
    }

done:
    if (uri.length()) {
        const char* p = uri.c_str();
        while (*p == '/' && *(p+1) == '/') {
            p++;
        }

        uri = std::string(p);
        auto ptr_uri = o->o_realProp(YAF_REQUEST_PROPERTY_NAME_URI, 
                ObjectData::RealPropUnchecked, "Yaf_Request_Http");

        *ptr_uri = String(uri);
        yaf_request_set_base_uri(o, base_uri, uri.c_str());
    }

    auto ptr_params = o->o_realProp(YAF_REQUEST_PROPERTY_NAME_PARAMS, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Http");
    *ptr_params = Array::Create();

    return o;
}

static Variant HHVM_METHOD(Yaf_Request_Http, getQuery, const Variant& name,
        const Variant& def) 
{
    if (name.isNull()) {
        return def;
    }

    if (!php_global(S_GET).toArray().exists(name)) {
        return def;
    }

    return php_global(S_GET).toArray()[name];
}

static Variant HHVM_METHOD(Yaf_Request_Http, getRequest, const Variant& name,
        const Variant& def) 
{
    if (name.isNull()) {
        return def;
    }

    if (!php_global(S_REQUEST).toArray().exists(name)) {
        return def;
    }

    return php_global(S_REQUEST).toArray()[name];
}

static Variant HHVM_METHOD(Yaf_Request_Http, getPost, const Variant& name,
        const Variant& def) 
{
    if (name.isNull()) {
        return def;
    }

    if (!php_global(S_POST).toArray().exists(name)) {
        return def;
    }

    return php_global(S_POST).toArray()[name];
}
 
static Variant HHVM_METHOD(Yaf_Request_Http, getCookie, const Variant& name,
        const Variant& def) 
{
    if (name.isNull()) {
        return def;
    }

    if (!php_global(S_COOKIE).toArray().exists(name)) {
        return def;
    }

    return php_global(S_COOKIE).toArray()[name];
}

static Variant HHVM_METHOD(Yaf_Request_Http, getFiles, const Variant& name,
        const Variant& def) 
{
    if (name.isNull()) {
        return def;
    }

    if (!php_global(S_FILES).toArray().exists(name)) {
        return def;
    }

    return php_global(S_FILES).toArray()[name];
}

static Variant HHVM_METHOD(Yaf_Request_Http, get, const Variant& name,
        const Variant& def) 
{
    if (name.isNull()) {
        return def;
    }

    auto paramsTmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_PARAMS, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Http");

    if (paramsTmp->isNull()) {
        return def;
    }

    Array& params = paramsTmp->toArrRef();
    if (params.exists(name)) {
        return params[name];
    }

    if (php_global(S_POST).toArray().exists(name)) {
        return php_global(S_POST).toArray()[name];
    }

    if (php_global(S_GET).toArray().exists(name)) {
        return php_global(S_GET).toArray()[name];
    }

    if (php_global(S_COOKIE).toArray().exists(name)) {
        return php_global(S_COOKIE).toArray()[name];
    }

    if (php_global(S_SERVER).toArray().exists(name)) {
        return php_global(S_SERVER).toArray()[name];
    }

    return def;
}
 
static bool HHVM_METHOD(Yaf_Request_Http, isXmlHttpRequest)
{
    if (!php_global(S_SERVER).toArray().exists(String("HTTP_X_REQUESTED_WITH"))) {
        return false;
    }

    Variant header = php_global(S_SERVER).toArray()[String("HTTP_X_REQUESTED_WITH")];
    if (!header.isString()) {
        return false;
    }

    std::string str_header = header.toString().toCppString();
    if (strncasecmp(str_header.c_str(), "XMLHttpRequest", str_header.length())) {
        return false;
    } 

    return true;
}

static void HHVM_METHOD(Yaf_Request_Http, __construct, 
        const Variant& request_uri, const Variant& base_uri)
{
    (void)yaf_request_http_instance(this_, request_uri, base_uri);
}

static void HHVM_METHOD(Yaf_Request_Http, __clone)
{
}


void YafExtension::_initYafRequestHttpClass()
{
    HHVM_ME(Yaf_Request_Http, getQuery);
    HHVM_ME(Yaf_Request_Http, getRequest);
    HHVM_ME(Yaf_Request_Http, getPost);
    HHVM_ME(Yaf_Request_Http, getCookie);
    HHVM_ME(Yaf_Request_Http, getFiles);
    HHVM_ME(Yaf_Request_Http, get);
    HHVM_ME(Yaf_Request_Http, isXmlHttpRequest);
    HHVM_ME(Yaf_Request_Http, __construct);
    HHVM_ME(Yaf_Request_Http, __clone);
}

}
