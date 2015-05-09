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


Variant yaf_config_instance(Object& object, 
        const Variant& filename, const Variant& section)
{
    if (filename.isString()) {
        String str_filename = filename.toString();
        if (strncasecmp(str_filename.c_str() + str_filename.length() - 3, "ini", 3) == 0) {
            /*bool cache = g_yaf_local_data.get()->cache_config;
            if (cache) {
                return yaf_config_ini_unserialize(object, filename, section);
            }*/

            Variant o = yaf_config_ini_instance(object, filename, section);
            /*if (cache) {
                yaf_config_ini_serialize(object, filename, section);
            }*/
            return o;
        }
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
