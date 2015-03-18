/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-12 10:29:15
*   description  ：
*
=============================================*/
#include "hphp/runtime/base/class-info.h"
#include "hphp/runtime/ext/std/ext_std_classobj.h"

#include "yaf_application.h"
#include "yaf_config.h"
#include "ext_yaf.h"
#include "error.h"
#include "yaf_router.h"
#include "yaf_request.h"
#include "yaf_bootstrap.h"


namespace HPHP{

bool yaf_application_is_module_name(const String& module)
{
    Variant var_app = get_app();
    if (!var_app.isObject()) {
        return false;
    }

    Object o = var_app.toObject();
    auto ptr_modules = o->o_realProp(YAF_APPLICATION_PROPERTY_NAME_MODULES, 
            ObjectData::RealPropUnchecked, "Yaf_Application");
    if (!ptr_modules->isArray()) {
        return false;
    }

    Array& tmp = ptr_modules->toArrRef();
    ArrayIter iter = tmp.begin();
    while (!iter.end()) {
        Variant key = iter.first();
        Variant value = iter.second();

        if (!value.isString()) {
            iter.next();
            continue;
        }

        String& str_value = value.toStrRef();
        if (strncasecmp(str_value.c_str(), module.c_str(), module.length()) == 0) {
            return true;
        }

        iter.next();
    }

    return false;
}

static int yaf_application_init_loader()
{
    Variant var_global_library = init_null_variant;
    if (g_yaf_local_data.get()->global_library.length()) {
        var_global_library = String(g_yaf_local_data.get()->global_library);
    }
 
    Variant loader = init_null_variant;
    Variant var_local_library = init_null_variant;
    if (g_yaf_local_data.get()->local_library.length()) {
        var_local_library = String(g_yaf_local_data.get()->local_library);
    } else {
         char local_library[8192];
         snprintf(local_library, sizeof(local_library), "%s%c%s", 
                 g_yaf_local_data.get()->directory.c_str(), 
                 DEFAULT_SLASH_CHAR, YAF_LIBRARY_DIRECTORY_NAME);
        var_local_library = String(local_library);

    }

    loader = yaf_loader_instance(NULL, var_local_library, var_global_library);
    if (loader.isNull()) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "Initialization of application auto loader failed");
        return HHVM_YAF_FAILED;
    }

    return HHVM_YAF_SUCCESS;
}

static int yaf_application_parse_system(const Array& config)
{
    if (!config.exists(String("system"))) {
        return HHVM_YAF_SUCCESS;
    }

    Variant var_system = config[String("system")];
    if (!var_system.isArray()) {
        return HHVM_YAF_SUCCESS;
    }

    Array& arr_system = var_system.toArrRef();
    ArrayIter iter = arr_system.begin();

    char name[128];
    while(!iter.end()) {
        Variant first = iter.first();
        Variant second = iter.second();
        snprintf(name, sizeof(name), "%s.yaf", first.toString().c_str());

        String value = second.toString();
        //TODO modify runtime ini config
        // zend_alter_ini_entry(name, len + 1, Z_STRVAL_PP(value), Z_STRLEN_PP(value), PHP_INI_USER, PHP_INI_STAGE_RUNTIME);
        iter.next();
    }

    return HHVM_YAF_SUCCESS;
}

static int yaf_application_parse_module(const Array& config)
{
    if (!config.exists(String("modules")) ||
            config[String("modules")].isString() == false ||
            config[String("modules")].toString().length() == 0) {
         
        Array arr_modules = Array::Create();
        String str_module = String(g_yaf_local_data.get()->default_module);
        arr_modules.append(str_module);

        g_yaf_local_data.get()->modules = arr_modules;
        return HHVM_YAF_SUCCESS;
    }

    Array arr_modules = Array::Create();
    String str_modules = config[String("modules")].toString();
    char* ptr_modules_dup = strdup(str_modules.c_str());

    char* save_ptr = NULL;
    char* seg = strtok_r(ptr_modules_dup, ",", &save_ptr);
    while (seg) {
        if (seg && strlen(seg)) {
            arr_modules.append(String(seg));       
        }

        seg = strtok_r(NULL, ",", &save_ptr);
    }

    free(ptr_modules_dup);
    g_yaf_local_data.get()->modules = arr_modules;
    return HHVM_YAF_SUCCESS;
}

static int yaf_application_parse_dispatcher(const Array& config)
{
    if (!config.exists(String("dispatcher"))) {
        g_yaf_local_data.get()->default_module = YAF_ROUTER_DEFAULT_MODULE;
        g_yaf_local_data.get()->default_controller = YAF_ROUTER_DEFAULT_CONTROLLER;
        g_yaf_local_data.get()->default_action = YAF_ROUTER_DEFAULT_ACTION;
        return HHVM_YAF_SUCCESS;
    }

    Variant var_dispatcher = config[String("dispatcher")];
    if (!var_dispatcher.isArray()) {
        g_yaf_local_data.get()->default_module = YAF_ROUTER_DEFAULT_MODULE;
        g_yaf_local_data.get()->default_controller = YAF_ROUTER_DEFAULT_CONTROLLER;
        g_yaf_local_data.get()->default_action = YAF_ROUTER_DEFAULT_ACTION;
        return HHVM_YAF_SUCCESS;
    }

    //defaultModule
    g_yaf_local_data.get()->default_module = YAF_ROUTER_DEFAULT_MODULE;
    Array& arr_dispatcher = var_dispatcher.toArrRef();
    if (arr_dispatcher.exists(String("defaultModule"))) {
        Variant var_module = arr_dispatcher[String("defaultModule")];
        if (var_module.isString()) {
            g_yaf_local_data.get()->default_module = var_module.toString().toCppString();
            transform(g_yaf_local_data.get()->default_module.begin(), 
                    g_yaf_local_data.get()->default_module.end(), 
                    g_yaf_local_data.get()->default_module.begin(), tolower);

            if (g_yaf_local_data.get()->default_module.length()) {
                g_yaf_local_data.get()->default_module[0] = toupper(g_yaf_local_data.get()->default_module[0]);
            }
        }
    } 

    //defaultController
    g_yaf_local_data.get()->default_controller = YAF_ROUTER_DEFAULT_CONTROLLER;
    if (arr_dispatcher.exists(String("defaultController"))) {
        Variant var_controller = arr_dispatcher[String("defaultController")];
        if (var_controller.isString()) {
            g_yaf_local_data.get()->default_controller = var_controller.toString().toCppString();
            transform(g_yaf_local_data.get()->default_controller.begin(), 
                    g_yaf_local_data.get()->default_controller.end(), 
                    g_yaf_local_data.get()->default_controller.begin(), tolower);

            if (g_yaf_local_data.get()->default_controller.length()) {
                g_yaf_local_data.get()->default_controller[0] = toupper(g_yaf_local_data.get()->default_controller[0]);
            }
        }
    } 

    //defaultAction
    g_yaf_local_data.get()->default_action = YAF_ROUTER_DEFAULT_ACTION;
    if (arr_dispatcher.exists(String("defaultAction"))) {
        Variant var_action = arr_dispatcher[String("defaultAction")];
        if (var_action.isString()) {
            g_yaf_local_data.get()->default_action = var_action.toString().toCppString();
            transform(g_yaf_local_data.get()->default_action.begin(), 
                    g_yaf_local_data.get()->default_action.end(), 
                    g_yaf_local_data.get()->default_action.begin(), tolower);
        }
    } 

    if (arr_dispatcher.exists(String("throwException"))) {
        Variant var_exception = arr_dispatcher[String("throwException")];
        g_yaf_local_data.get()->throw_exception = var_exception.toBoolean();
    }

    if (arr_dispatcher.exists(String("catchException"))) {
        Variant var_exception = arr_dispatcher[String("catchException")];
        g_yaf_local_data.get()->catch_exception = var_exception.toBoolean();
    }

    if (arr_dispatcher.exists(String("defaultRoute"))) {
        Variant var_route = arr_dispatcher[String("defaultRoute")];
        if (var_route.isArray()) {
            g_yaf_local_data.get()->default_route = var_route.toArray();
        }
    }

    return HHVM_YAF_SUCCESS;
}

static int yaf_application_parse_library(const Array& config)
{
    if (!config.exists(String("library"))) {
        return HHVM_YAF_SUCCESS;
    }

    Variant var_library = config[String("library")];
    if (var_library.isString()) {
        g_yaf_local_data.get()->local_library = var_library.toString().toCppString();
        return HHVM_YAF_SUCCESS;
    }

    if (var_library.isArray()) {
        Array& arr_library = var_library.toArrRef();
        if (arr_library.exists(String("directory"))) {
            Variant var_directory = arr_library[String("directory")];
            if (var_directory.isString()) {
                g_yaf_local_data.get()->local_library = var_directory.toString().toCppString();
            }
        }

        if (arr_library.exists(String("namespace"))) {
            Variant var_namespace = arr_library[String("namespace")];
            if (var_namespace.isString()) {
                std::string str_tmp;
                std::string str_src = var_namespace.toString().toCppString();
                for (size_t i = 0; i < str_src.length(); i++) {
                    if (str_src[i] == ',') {
                        str_tmp.push_back(DEFAULT_DIR_SEPARATOR);
                    } else if (str_src[i] != ' ') {
                        str_tmp.push_back(str_src[i]);
                    }
                }
                yaf_loader_register_namespace_single(str_tmp);
            }
        }
    }

    return HHVM_YAF_SUCCESS;
}

static int yaf_application_parse_option(const Array& config)
{
    Variant app;
    if (!config.exists(String("application"))) {
        if (!config.exists(String("yaf"))) {
            yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                    "not found application configure");
            return HHVM_YAF_NOT_FOUND_APPLICATION_CONF;
        }

        app = config[String("yaf")];
    } else {
        app = config[String("application")];
    }

    if (!app.isArray()) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                "Expected an array of application configure");
        return HHVM_YAF_APPLICATION_CONF_NOT_ARRAY;
    }

    Array& arr_app = app.toArrRef();
    if (!arr_app.exists(String("directory"))) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "expect directory entry in application conf");
        return HHVM_YAF_NOT_FOUND_APPLICATION_DIR;
    }

    //directory
    Variant var_directory = arr_app[String("directory")];
    if (!var_directory.isString() || 
            var_directory.toString().length() == 0) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "expect directory entry to be string");
        return HHVM_YAF_DIRECTORY_NOT_STRING;
    }

    g_yaf_local_data.get()->directory = var_directory.toString().toCppString();
    size_t directory_len = g_yaf_local_data.get()->directory.length();
    if (g_yaf_local_data.get()->directory[directory_len - 1] == DEFAULT_SLASH_CHAR) {
        g_yaf_local_data.get()->directory.erase(directory_len - 1);
    }

    //ext
    g_yaf_local_data.get()->ext = YAF_DEFAULT_EXT;
    if (arr_app.exists(String("ext"))) {
        Variant var_ext = arr_app[String("ext")];
        if (var_ext.isString()) {
            g_yaf_local_data.get()->ext = var_ext.toString().toCppString();
        } 
    }

    //boostrap
    if (arr_app.exists(String("bootstrap"))) {
        Variant var_bootstrap = arr_app[String("bootstrap")];
        if (var_bootstrap.isString()) {
            g_yaf_local_data.get()->bootstrap = var_bootstrap.toString().toCppString();
        }
    }

    //library
    int ret = yaf_application_parse_library(arr_app);
    if (ret != HHVM_YAF_SUCCESS) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "yaf_application_parse_library failed, ret:%d", ret);
        return ret;
    }

    //view
    g_yaf_local_data.get()->view_ext = YAF_DEFAULT_VIEW_EXT;
    if (arr_app.exists(String("view"))) {
        Variant var_view = arr_app[String("view")];
        if (!var_view.isArray()) {
            g_yaf_local_data.get()->view_ext = YAF_DEFAULT_VIEW_EXT;
        } else {
            Array& arr_view = var_view.toArrRef();
            if (arr_view.exists(String("ext"))) {
                Variant var_ext = arr_view[String("ext")];
                if (var_ext.isString()) {
                    g_yaf_local_data.get()->view_ext = var_ext.toString().toCppString();
                }
            }
        }
    } 

    //baseuri
    if (arr_app.exists(String("baseUri"))) {
        Variant var_baseuri = arr_app[String("baseUri")];
        if (var_baseuri.isString()) {
            g_yaf_local_data.get()->base_uri = var_baseuri.toString().toCppString();
        }
    }

    //dispatcher
    ret = yaf_application_parse_dispatcher(arr_app);
    if (ret != HHVM_YAF_SUCCESS) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "yaf_application_parse_dispatcher failed, ret:%d", ret);
        return ret;
    }

    //modules
    ret = yaf_application_parse_module(arr_app);
    if (ret != HHVM_YAF_SUCCESS) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "yaf_application_parse_modules failed, ret:%d", ret);
        return ret;
    }

    //system
    ret = yaf_application_parse_system(arr_app);
    if (ret != HHVM_YAF_SUCCESS) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "yaf_application_parse_system failed, ret:%d", ret);
        return ret;
    }

    return HHVM_YAF_SUCCESS;
}

extern Variant get_app()
{
    Array func = Array::Create();
    func.append("Yaf_Application");
    func.append("get_app");

    Array params = Array::Create();
    return vm_call_user_func(func, params);
}

static void set_app(const Object object)
{ 
    Array func = Array::Create();
    func.append("Yaf_Application");
    func.append("set_app");

    Array params = Array::Create();
    params.append(Variant(object));
    vm_call_user_func(func, params);
}

static void HHVM_METHOD(Yaf_Application, __construct, const Variant& config, 
        const Variant& section)
{
    Variant var_app = get_app();
    if (!var_app.isNull()) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "Only one application can be initialized");
        return;
    }

    String str_section;
    if (!section.isString() ||
            (section.isString() && section.toString().length() == 0)) {
        str_section = String(g_yaf_local_data.get()->environ.c_str());
    } else {
        str_section = section.toString();
    }

    Variant var_config = yaf_config_instance(NULL, config, str_section);
    if (var_config.isNull() || !var_config.isObject()) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "initialize yaf application config failed");
        return;
    }

    auto ob_config = var_config.toObject();
    if (!ob_config->o_instanceof("Yaf_Config_Abstract")) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "initialize yaf application config failed, not Yaf_config");
        return;
    }

    auto ptr_config = ob_config->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Abstract");
    if (!ptr_config->isArray()) {
        *ptr_config = Array::Create();
    }

    int ret = yaf_application_parse_option(ptr_config->toArray());
    if (ret != 0) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "initialize yaf application config failed, parse option failed");
        return;
    }

    Variant request = yaf_request_instance(NULL, 
            g_yaf_local_data.get()->base_uri.c_str());
    if (request.isNull()) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "initialize yaf request failed");
        return ;
    }

    if (g_yaf_local_data.get()->base_uri.length()) {
        g_yaf_local_data.get()->base_uri.clear();
    }

    Variant dispatcher = yaf_dispatcher_instance(NULL);
    if (dispatcher.isNull() || !dispatcher.isObject()) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "initialize yaf dispatcher failed, create instance of dispatcher failed");
        return;
    }

    if (!dispatcher.toObject()->o_instanceof("Yaf_Dispatcher")) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "initialize yaf dispatcher failed, not Yaf_Dispatcher instance");
        return;
    }

    Object o_dispatcher = dispatcher.toObject();
    yaf_dispatcher_set_request(&o_dispatcher,request);

    auto ptr_app_config = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_CONFIG, 
            ObjectData::RealPropUnchecked, "Yaf_Application");
    *ptr_app_config = ob_config;

    auto ptr_dispatcher = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_DISPATCHER, 
            ObjectData::RealPropUnchecked, "Yaf_Application");
    *ptr_dispatcher = o_dispatcher;

    ret = yaf_application_init_loader();
    if (ret != 0) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "Initialization of application auto loader failed");
        return;
    }

    auto ptr_run = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_RUN, 
            ObjectData::RealPropUnchecked, "Yaf_Application");
    *ptr_run = Variant(false);

    auto ptr_env = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_ENV, 
            ObjectData::RealPropUnchecked, "Yaf_Application");
    *ptr_env = String(g_yaf_local_data.get()->environ);

    auto ptr_modules = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_MODULES, 
            ObjectData::RealPropUnchecked, "Yaf_Application");
    if (g_yaf_local_data.get()->modules.isInitialized()) {
        /*Array& tmp = g_yaf_local_data.get()->modules.toArrRef();
        
        ArrayIter iter = tmp.begin();
        while (!iter.end()) {
            Variant first = iter.first();
            Variant second = iter.second();

            raise_warning("yaf_local_data modules, "\
                    "key:%s value:%s", first.toString().c_str(),
                    second.toString().c_str());
            iter.next();
        }
        */
        *ptr_modules = g_yaf_local_data.get()->modules;
    } else {
        *ptr_modules = init_null_variant;
    }


    //raise_warning("initialize yaf_application succ");
    //*ptr_app = this_;
    set_app(this_);
    return;
}

static Variant HHVM_METHOD(Yaf_Application, run)
{
    auto ptr_running = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_RUN, 
            ObjectData::RealPropUnchecked, "Yaf_Application");
    if (ptr_running->isBoolean() && 
            ptr_running->toBoolean()) {

        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "An application instance already run");
        return true; 
    }

    *ptr_running = Variant(true);

    auto ptr_dispatcher = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_DISPATCHER, 
            ObjectData::RealPropUnchecked, "Yaf_Application");
    Object object = ptr_dispatcher->toObject();
    try {
    Variant response = yaf_dispatcher_dispatch(&object);
    if (!response.isNull()) {
        return response;
    }
    } catch (Object& e) {
        
        raise_warning("catch object e");
    } catch (Exception& e) {
        raise_warning("catch exception e");
    }

    return false;
}

static TypedValue* HHVM_MN(Yaf_Application, execute)(ActRec* ar) 
{
    String func_name(getArg<KindOfString>(ar, 0)); 

    Array params = Array::Create();
    for (int i = 1; i < ar->numArgs(); i++) {
        params.append(getArg<KindOfRef>(ar, i));
    }

    return arReturn(ar,vm_call_user_func(func_name, params));
}

static Variant HHVM_METHOD(Yaf_Application, environ)
{
    auto ptr_environ = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_ENV, 
            ObjectData::RealPropUnchecked, "Yaf_Application");
    return ptr_environ->toString();
}

static Variant HHVM_METHOD(Yaf_Application, bootstrap)
{
    Array args = Array::Create();
    Object o;

    std::string bootstrap_path;
    if (g_yaf_local_data.get()->bootstrap.length()) {
        bootstrap_path = g_yaf_local_data.get()->bootstrap;
    } else {
        bootstrap_path = g_yaf_local_data.get()->directory + DEFAULT_SLASH_STR + 
            YAF_DEFAULT_BOOTSTRAP + "." +g_yaf_local_data.get()->ext;
    }

    bool ret = yaf_loader_import(bootstrap_path.c_str(), 
            bootstrap_path.length(), 0);
    if (ret == false) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "yaf_loader_import %s failed", bootstrap_path.c_str());
        return false;
    }

    o = createObject("bootstrap", args);
    if (o.isNull() || !o->o_instanceof("Yaf_Bootstrap_Abstract")) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "can't found bootstrap in %s", bootstrap_path.c_str());
        return false;
    }

    auto ptr_dispatcher = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_DISPATCHER, 
            ObjectData::RealPropUnchecked, "Yaf_Application");

    Array arr = Array::Create();
    Class* cls = o->getVMClass(); 

    Func* const* methods = cls->preClass()->methods();
    size_t const numMethods = cls->preClass()->numMethods();
    for (Slot i = 0; i < numMethods; ++i) {
        const Func* m = methods[i];
        if (m->isGenerated()) {
            continue;
        }

        arr.append(String(m->nameStr()));
    }

    for (Slot i = cls->traitsBeginIdx(); i < cls->traitsEndIdx(); ++i) {
        const Func* m = cls->getMethod(i);
        if (m->isGenerated()) {
            continue;
        }

        arr.append(String(m->nameStr()));
    }

    ArrayIter iter = arr.begin();
    while (!iter.end()) {
        String func_name = iter.second();
        if (strncasecmp(func_name.c_str(), YAF_BOOTSTRAP_INITFUNC_PREFIX, 
                    sizeof(YAF_BOOTSTRAP_INITFUNC_PREFIX) - 1)) {
            iter.next();
            continue;
        }

        Array arr_func = Array::Create();
        arr_func.append(o);
        arr_func.append(func_name);

        Array params = Array::Create();
        params.append(*ptr_dispatcher);

        vm_call_user_func(arr_func, params);
        iter.next();
    }

    return this_;
}

static Variant HHVM_METHOD(Yaf_Application, getConfig)
{
    auto ptr_config = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_CONFIG, 
            ObjectData::RealPropUnchecked, "Yaf_Application");
    return *ptr_config;
}

static Variant HHVM_METHOD(Yaf_Application, getModules)
{
    auto ptr_modules = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_MODULES, 
            ObjectData::RealPropUnchecked, "Yaf_Application");
    return *ptr_modules;
}

static Variant HHVM_METHOD(Yaf_Application, getDispatcher)
{
    auto ptr_dispatcher = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_DISPATCHER, 
            ObjectData::RealPropUnchecked, "Yaf_Application");
    return *ptr_dispatcher;
}

static Variant HHVM_METHOD(Yaf_Application, setAppDirectory,
       const String& directory)
{
    if (!directory.length()) {
        return false;
    }

    if (!IS_ABSOLUTE_PATH(directory)) {
        return false;
    }

    g_yaf_local_data.get()->directory = directory.toCppString();
    return this_;
}

static Variant HHVM_METHOD(Yaf_Application, getAppDirectory)
{
    return String(g_yaf_local_data.get()->directory);
}

static Variant HHVM_METHOD(Yaf_Application, getLastErrorNo)
{
    auto ptr_error = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_ERRNO, 
            ObjectData::RealPropUnchecked, "Yaf_Application");
    return *ptr_error;
}

static Variant HHVM_METHOD(Yaf_Application, getLastErrorMsg)
{
    auto ptr_error = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_ERRMSG, 
            ObjectData::RealPropUnchecked, "Yaf_Application");
    return *ptr_error;
}

static Variant HHVM_METHOD(Yaf_Application, clearLastError)
{
    auto ptr_errno = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_ERRNO, 
            ObjectData::RealPropUnchecked, "Yaf_Application");

    *ptr_errno = 0;

    auto ptr_error = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_ERRMSG, 
            ObjectData::RealPropUnchecked, "Yaf_Application");
    *ptr_error = String("");

    return this_;
}

static void HHVM_METHOD(Yaf_Application, __destruct)
{
    set_app(Object());        
}

static void HHVM_METHOD(Yaf_Application, __clone)
{
}

static void HHVM_METHOD(Yaf_Application, __sleep)
{
}

static void HHVM_METHOD(Yaf_Application, __wakeup)
{
}

void YafExtension::_initYafApplicationClass()
{
    HHVM_ME(Yaf_Application, __construct);
    HHVM_ME(Yaf_Application, run);
    HHVM_ME(Yaf_Application, execute);
    HHVM_ME(Yaf_Application, environ);
    HHVM_ME(Yaf_Application, bootstrap);
    HHVM_ME(Yaf_Application, getConfig);
    HHVM_ME(Yaf_Application, getModules);
    HHVM_ME(Yaf_Application, getDispatcher);
    HHVM_ME(Yaf_Application, setAppDirectory);
    HHVM_ME(Yaf_Application, getAppDirectory);
    HHVM_ME(Yaf_Application, getLastErrorNo);
    HHVM_ME(Yaf_Application, getLastErrorMsg);
    HHVM_ME(Yaf_Application, clearLastError);

    HHVM_ME(Yaf_Application, __destruct);
    HHVM_ME(Yaf_Application, __clone);
    HHVM_ME(Yaf_Application, __sleep);
    HHVM_ME(Yaf_Application, __wakeup);
}

}
