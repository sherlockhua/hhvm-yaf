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
#include "hphp/runtime/ext/std/ext_std_classobj.h"
#include "hphp/runtime/ext/extension.h"
#include "ext_yaf.h"
#include "yaf_router.h"
#include "yaf_bootstrap.h"

namespace HPHP { 


IMPLEMENT_REQUEST_LOCAL(YafRequestData,
                      g_yaf_local_data);

Object createObject(const String& obj_typename, Array args) {

    if (!HHVM_FN(class_exists)(obj_typename)) {
        //raise_warning("yaf Class %s does not exist",
        //          obj_typename.data());
        return Object();
    }

    return create_object(obj_typename, args);
}

bool function_exists(const Object& o, const String& method)
{
    const Func* func = o->methodNamed(method.get());
    return func != nullptr;
}

void YafExtension::moduleInit()
{
    _initYafActionClass();
    _initYafControllerClass();
    _initYafRequestClass();
    _initYafResponseClass();

    _initYafApplicationClass();
    _initYafDispatcherClass();

    _initYafViewClass();
    _initYafConfigClass();
    _initYafAutoloadClass();

    _initYafRouterClass();
    _initYafRegistryClass();
    _initYafExceptionClass();

    _initYafSessionClass();
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

    g_yaf_local_data.get()->ext                = YAF_DEFAULT_EXT; 
    g_yaf_local_data.get()->view_ext           = YAF_DEFAULT_VIEW_EXT;
    g_yaf_local_data.get()->default_module     = YAF_ROUTER_DEFAULT_MODULE;
    g_yaf_local_data.get()->default_controller = YAF_ROUTER_DEFAULT_CONTROLLER;
    g_yaf_local_data.get()->default_action     = YAF_ROUTER_DEFAULT_ACTION;
    //g_yaf_local_data.get()->bootstrap          = YAF_DEFAULT_BOOTSTRAP;

}

void YafExtension::moduleLoad(Hdf config)
{
    Hdf conf = config["ext_conf"]["yaf"];
    _loadYafConf(conf);
}

void YafExtension::moduleLoad(const IniSetting::Map& ini, Hdf config)
{
    Hdf conf = config["ext_conf"]["yaf"];
    _loadYafConf(conf);
}

void YafExtension::threadInit() {
    Extension* ext = Extension::GetExtension("yaf");
    assert(ext);
    YafRequestData* data = g_yaf_local_data.get();
    IniSetting::Bind(ext, IniSetting::PHP_INI_ALL,
                     "yaf.action_prefer", "0",
                     &data->action_prefer);

    IniSetting::Bind(ext, IniSetting::PHP_INI_ALL,
                     "yaf.library", "",
                     &data->global_library);

    IniSetting::Bind(ext, IniSetting::PHP_INI_ALL,
                     "yaf.lowcase_path", "0",
                     &data->lowcase_path);

    IniSetting::Bind(ext, IniSetting::PHP_INI_ALL,
                     "yaf.use_spl_autoload", "0",
                     &data->use_spl_autoload);

    IniSetting::Bind(ext, IniSetting::PHP_INI_ALL,
                     "yaf.forward_limit", "5",
                     &data->forward_limit);

    IniSetting::Bind(ext, IniSetting::PHP_INI_ALL,
                     "yaf.name_suffix", "1",
                     &data->name_suffix);

    IniSetting::Bind(ext, IniSetting::PHP_INI_ALL,
                     "yaf.name_separator", "",
                     &data->name_separator);

    IniSetting::Bind(ext, IniSetting::PHP_INI_ALL,
                     "yaf.cache_config", "0",
                     &data->cache_config);

    IniSetting::Bind(ext, IniSetting::PHP_INI_ALL,
                     "yaf.st_compatible", "0",
                     &data->st_compatible);

    IniSetting::Bind(ext, IniSetting::PHP_INI_ALL,
                     "yaf.environ", "product",
                     &data->environ);

#ifdef YAF_HAVE_NAMESPACE
    IniSetting::Bind(ext, IniSetting::PHP_INI_ALL,
                     "yaf.use_namespace", "0",
                     &data->use_namespace);
#endif
}

YafExtension s_yaf_extension;
HHVM_GET_MODULE(yaf);

}
