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
    loadSystemlib();
}


void YafExtension::_loadYafConf(Hdf conf)
{

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
