/*===========================================
*   Copyright (C) 2014 All rights reserved.
*   
*   company      : xiaomi
*   author       : root
*   email        : root@xiaomi.com
*   date         ：2014-12-13 09:48:47
*   description  ：
*
=============================================*/
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/ext/extension.h"
#include "ext_yaf.h"

namespace HPHP { 


IMPLEMENT_REQUEST_LOCAL(YafRequestData,
                      g_yaf_local_data);

void YafExtension::moduleInit()
{
    _initYafActionClass();
    _initYafControllerClass();
    _initYafRequestClass();

    loadSystemlib();
}


void YafExtension::_loadYafConf(Hdf conf)
{
    g_yaf_local_data.get()->global_library = conf["library"].configGetString("");
    g_yaf_local_data.get()->action_prefer = conf["action_prefer"].configGetBool(false);
    g_yaf_local_data.get()->lowcase_path = conf["lowcase_path"].configGetBool(false);
    g_yaf_local_data.get()->use_spl_autoload = conf["use_spl_autoload"].configGetBool(false);
    g_yaf_local_data.get()->forward_limit = conf["forward_limit"].configGetInt32(5);
    g_yaf_local_data.get()->name_suffix = conf["name_suffix"].configGetBool(true);
    g_yaf_local_data.get()->name_separator = conf["name_separator"].configGetString("");
    g_yaf_local_data.get()->cache_config = conf["cache_config"].configGetBool(false);
    g_yaf_local_data.get()->st_compatible = conf["st_compatible"].configGetBool(false);
/* {{{ This only effects internally */
    g_yaf_local_data.get()->st_compatible = conf["st_compatible"].configGetBool(false);
/* }}} */
    g_yaf_local_data.get()->environ = conf["environ"].configGetString("product");

#ifdef YAF_HAVE_NAMESPACE
    g_yaf_local_data.get()->use_namespace = conf["use_namespace"].configGetBool(false);
#endif

}

void YafExtension::moduleLoad(Hdf config)
{
    Hdf conf = config["ext_conf"]["yaf"];
    _loadYafConf(conf);
}

void YafExtension::moduleLoad(const IniSetting::Map& ini, Hdf config)
{
    Hdf conf = config["ext_conf"]["xm_rpc"];
    _loadYafConf(conf);
}

YafExtension s_yaf_extension;
HHVM_GET_MODULE(yaf);

}
