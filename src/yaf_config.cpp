/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-22 16:40:02
*   description  ：
*
=============================================*/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "yaf_config.h"
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/ext/extension.h"
#include "ext_yaf.h"

namespace HPHP {

static bool yaf_config_ini_modified(const Variant& filename, long ctime)
{
    struct stat buf;
    stat(filename.toString().c_str(), &buf);
    if (buf.st_ctime != ctime) {
        return true;
    }

    return false;
}

Variant yaf_config_ini_unserialize(Object* object, 
        const Variant& filename, const Variant& section)
{
    Array& configs = g_yaf_local_data.get()->configs;
    char cache_key[8192];

    snprintf(cache_key, sizeof(cache_key), "%s#%s", 
        filename.toString().c_str(), section.toString().c_str());

    if (configs.exists(String(cache_key))) {
        Variant data = configs[String(cache_key)];
        if (!data.isArray()) {
            return init_null_variant;
        }

        Array& arr_data = data.toArrRef();
        if (arr_data.exists(String(YAF_CONFIG_CACHE_CTIME))) {
            return init_null_variant;
        }

        if (arr_data.exists(String(YAF_CONFIG_CACHE_DATA))) {
            return init_null_variant;
        }

        Variant var_ctime = arr_data[String(YAF_CONFIG_CACHE_CTIME)];
        Variant var_config = arr_data[String(YAF_CONFIG_CACHE_DATA)];

        if (!var_ctime.isInteger()) {
            return init_null_variant;
        }

        if (!var_config.isArray()) {
            return init_null_variant;
        }

        long ctime = var_ctime.toInt64();
        if (yaf_config_ini_modified(filename, ctime)) {
            return init_null_variant;
        }
        return yaf_config_ini_instance(object, var_config.toArray(), section);
    }

    return init_null_variant;
}

Variant yaf_config_ini_serialize(Object* object, 
        const Variant& filename, const Variant& section)
{
    return init_null_variant;
}


Variant yaf_config_instance(Object* object, 
        const Variant& filename, const Variant& section)
{
    if (filename.isString()) {
        bool cache = g_yaf_local_data.get()->cache_config;
        if (cache) {
            return yaf_config_ini_unserialize(object, filename, section);
        }

        Variant o = yaf_config_ini_instance(object, filename, section);
        if (cache) {
            yaf_config_ini_serialize(object, filename, section);
        }

        return o;
    }

    if (filename.isArray()) {
        return yaf_config_simple_instance(object, filename, true);  
    }

    yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
        "Expects a string or an array as parameter");
    return init_null_variant;
}

static void HHVM_METHOD(Yaf_Config_Abstract, __construct)
{}


void YafExtension::_initYafConfigClass()
{
    HHVM_ME(Yaf_Config_Abstract, __construct);

    _initYafConfigSimpleClass();
    _initYafConfigIniClass();
}

void yaf_config_cursor::sweep() {
  Close();
}



}
