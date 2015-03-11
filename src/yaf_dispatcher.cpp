/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-12 11:03:59
*   description  ：
*
=============================================*/
#include "yaf_dispatcher.h"
#include "ext_yaf.h"
#include "yaf_view.h"
#include "yaf_application.h"
#include "yaf_response.h"
#include "yaf_request.h"
#include "yaf_router.h"
#include "yaf_action.h"
#include "yaf_controller.h"

namespace HPHP {


static Variant get_instance()
{
    Array func = Array::Create();
    func.append("Yaf_Dispatcher");
    func.append("getInstance");

    Array params = Array::Create();
    return vm_call_user_func(func, params);
}

static Variant set_instance(const Object& o)
{
    Array func = Array::Create();
    func.append("Yaf_Dispatcher");
    func.append("setInstance");

    Array params = Array::Create();
    params.append(o);
    return vm_call_user_func(func, params);
}

Variant yaf_dispatcher_instance(Object* object)
{
    Variant router;

    Variant instance = get_instance();
    if (instance.isObject()
            && instance.toObject()->o_instanceof(String("Yaf_Dispatcher"))) {
        return instance;
    }

    if (object) {
        instance = object;
        return instance;
    } else {
        Array params = Array::Create();
        instance = createObject(String("Yaf_Dispatcher"), params);
    }

    Object o_instance = instance.toObject();
    auto ptr_plugins = o_instance->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_PLUGINS, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    *ptr_plugins = Array::Create();
    router   = yaf_router_instance(NULL);

    auto ptr_router = o_instance->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_ROUTER, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");
    *ptr_router = router;

    auto ptr_module = o_instance->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_MODULE, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");
    *ptr_module = String(g_yaf_local_data.get()->default_module);

    auto ptr_controller = o_instance->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_CONTROLLER, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");
    *ptr_controller = String(g_yaf_local_data.get()->default_controller);

    auto ptr_action = o_instance->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_ACTION, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");
    *ptr_action = String(g_yaf_local_data.get()->default_action);


    set_instance(o_instance);

    return instance;
}

static Variant yaf_dispatcher_get_action(const char* app_dir, 
        const Object& controller, const String& module, int def_module, 
        const String& action, const Array& params)
{
    auto ptr_action_map = controller->o_realProp(YAF_CONTROLLER_PROPERTY_NAME_ACTIONS,
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");

    char action_upper[1024];
    snprintf(action_upper, sizeof(action_upper), "%s", action.c_str());
    action_upper[0] = toupper(action_upper[0]);

    if (ptr_action_map->isArray()) {
        Array& action_map = ptr_action_map->toArrRef();
        if (!action_map.exists(action)) {
            raise_warning("there's no method %sAction in %s::$%s", 
                    action.c_str(), controller->getVMClass()->name()->data(), 
                    YAF_CONTROLLER_PROPERTY_NAME_ACTIONS);
            return init_null_variant;
        }

        Variant var_value = action_map[action];
        if (!var_value.isString()) {
            raise_warning("wrong value type, there's no method %sAction in %s::$%s", 
                    action.c_str(), controller->getVMClass()->name()->data(), 
                    YAF_CONTROLLER_PROPERTY_NAME_ACTIONS);
            return init_null_variant;
        }

        char action_path[1024];
        snprintf(action_path, sizeof(action_path), "%s%c%s", 
                app_dir, DEFAULT_SLASH_CHAR, var_value.toString().c_str());

        raise_warning("action class path is %s", action_path);
        if (!yaf_loader_import(action_path, strlen(action_path), 0)) {
            raise_warning("can't open action script:%s", action_path);
            return init_null_variant;
        }

        char action_class[1024];
        if (g_yaf_local_data.get()->name_suffix) {
            snprintf(action_class, sizeof(action_class), "%s%sAction",
                    action_upper, g_yaf_local_data.get()->name_separator.c_str());
        } else {
            snprintf(action_class, sizeof(action_class), "Action%s%s",
                    g_yaf_local_data.get()->name_separator.c_str(), action_upper);
        }

        raise_warning("action class name is %s", action_class);
	    Object o = createObject(String(action_class), params);
        if (!o->o_instanceof("Yaf_Action_Abstract")) {
            raise_warning("Action must extends from Yaf_Action_Abstract");
            return init_null_variant;
        }

        return o;
    }

    if (g_yaf_local_data.get()->st_compatible) {
        char *p;
        char directory[1024];
        char class_name[1024];
        /**
         * upper Action Name
         * eg: Index_sub -> Index_Sub
         */
        p = action_upper;
        *(p) = toupper(*p);
        while (*p != '\0') {
            if (*p == '_'
#ifdef YAF_HAVE_NAMESPACE
                    || *p == '\\'
#endif
               ) {
                if (*(p+1) != '\0') {
                    *(p+1) = toupper(*(p+1));
                    p++;
                }
            }
            p++;
        }

        if (def_module) {
            snprintf(directory, sizeof(directory), "%s%c%s", 
                    app_dir, DEFAULT_SLASH_CHAR, "actions");
        } else {
            snprintf(directory, sizeof(directory), 
                    "%s%c%s%c%s%c%s", app_dir, DEFAULT_SLASH_CHAR,
                    "modules", DEFAULT_SLASH_CHAR, module.c_str(), DEFAULT_SLASH_CHAR, "actions");
        }

        if (g_yaf_local_data.get()->name_suffix) {
            snprintf(class_name, sizeof(class_name), "%s%sAction", 
                    action_upper, g_yaf_local_data.get()->name_separator.c_str());
        } else {
            snprintf(class_name, sizeof(class_name), "Action%s%s", 
                    g_yaf_local_data.get()->name_separator.c_str(), action_upper);
        }

        Object o = createObject(String(class_name), params);
        if (!o->o_instanceof("Yaf_Action_Abstract")) {
            int ret = yaf_internal_autoload(action_upper, (int)strlen(action_upper),
                    (char**)&directory);
            if (ret != HHVM_YAF_SUCCESS) {
                raise_warning("yaf_internal_autoload failed, ret:%d, "\
                        "failed open action script:%s", ret, directory);
                return init_null_variant;
            }

            o = createObject(String(class_name), params);
            if (!o->o_instanceof("Yaf_Action_Abstract")) {
                return init_null_variant;
            }
        }

        return o;
    }

    return init_null_variant;
}

static Variant yaf_dispatcher_get_controller(const char* app_dir, 
        const char* module, const char* controller, int is_def_module, 
        const Array& args)
{
    char directory[8192];
    if (is_def_module) {
        snprintf(directory, sizeof(directory), "%s%c%s", app_dir, 
                DEFAULT_SLASH_CHAR, YAF_CONTROLLER_DIRECTORY_NAME);
    } else {
        snprintf(directory, sizeof(directory), "%s%c%s%c%s%c%s", 
                app_dir, DEFAULT_SLASH_CHAR, YAF_MODULE_DIRECTORY_NAME, 
                DEFAULT_SLASH_CHAR, module, DEFAULT_SLASH_CHAR, 
                YAF_CONTROLLER_DIRECTORY_NAME);
    }

    raise_warning("get controller, module:%s control:%s def:%d",
            module, controller, is_def_module);

	char class_name[8192];
	if (g_yaf_local_data.get()->name_suffix) {
		snprintf(class_name, sizeof(class_name), "%s%s%s", 
				controller, g_yaf_local_data.get()->name_separator.c_str(), "Controller");
	} else {
		snprintf(class_name, sizeof(class_name), "%s%s%s", 
				"Controller", g_yaf_local_data.get()->name_separator.c_str(), controller);
	}
    //TODO
    //

	//Array params = Array::Create();
	Object o = createObject(String(class_name), args);
	if (!o->o_instanceof("Yaf_Controller_Abstract")) {
		int ret = yaf_internal_autoload(controller, strlen(controller), (char**)&directory);
		if (ret != HHVM_YAF_SUCCESS) {
            raise_warning("yaf_internal_autoload failed, ret:%d", ret);
			return init_null_variant;
		}

		o = createObject(String(class_name), args);
		if (!o->o_instanceof("Yaf_Controller_Abstract")) {
			return init_null_variant;
		}
	}

    Object request = args[0].toObject();
    auto ptr_req_module = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_MODULE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    auto ptr_ctl_module = o->o_realProp(YAF_CONTROLLER_PROPERTY_NAME_REQUEST, 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    *ptr_ctl_module = *ptr_req_module;

    /*
    if (!o->o_instanceof("Yaf_Action_Abstract")) {
        Array func = Array::Create();
        func.append(control);
        func.append(String("init"));

        Array arr_params = Array::Create();
        vm_call_user_func(func, arr_params);
    }
    */

	return o;
}

static int yaf_dispatcher_get_call_parameters(const Object& request, 
            const Func* func, Array& params)
{
    auto ptr_args = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_PARAMS, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    if (!ptr_args->isArray()) {
        return HHVM_YAF_SUCCESS;
    }

    Array& arr_args = ptr_args->toArrRef();
    uint32_t func_params_size = func->numNamedLocals();

    raise_warning("get call parameters, count:%u", func_params_size);

    for (uint32_t i = 0; i < func_params_size; i++) {

        const StringData* params_name = func->localVarName(i);
        const char* ptr_params_name = params_name->data();

        if (arr_args.exists(String(ptr_params_name))) {
            params.append(arr_args[String(ptr_params_name)]);
        } else {
            ArrayIter iter = arr_args.begin();
            while (!iter.end()) {
                Variant first = iter.first();
                Variant second = iter.second();

                if (strncasecmp(first.toString().c_str(), ptr_params_name, 
                            first.toString().length()) == 0) {
                    params.append(second);
                }
                iter.next();
            }
        }
    }

    return HHVM_YAF_SUCCESS;
}

static int yaf_dispatcher_handle(const Object& object, const Object& request, 
        const Object& response, const Object& view)
{
    std::string app_dir = g_yaf_local_data.get()->directory;
    yaf_request_set_dispatched(request, 1);
    if (!app_dir.length()) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "application.directory must be set first");
        return HHVM_YAF_FAILED;
    }

    int is_def_module = 0;
    auto ptr_module = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_MODULE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    auto ptr_controller = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_CONTROLLER, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    auto ptr_dispatcher_module = object->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_MODULE, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    auto ptr_dispatcher_controller = object->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_CONTROLLER, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    if (!ptr_module->isString() ||
            ptr_module->toString().length() == 0) {
        yaf_trigger_error(YAF_ERR_DISPATCH_FAILED, "unexpect a empty module");
        return HHVM_YAF_FAILED;
    }

    bool ret = yaf_application_is_module_name(ptr_module->toString());
    if (!ret) {
        yaf_trigger_error(YAF_ERR_NOTFOUND_MODULE, "not found module:%s",
                ptr_module->toString().c_str());
        return HHVM_YAF_FAILED;
    }

    if (!ptr_controller->isString() ||
            ptr_controller->toString().length() == 0) {
        yaf_trigger_error(YAF_ERR_DISPATCH_FAILED, "unexpect a empty controller");
        return HHVM_YAF_FAILED;
    }

    if (strncasecmp(ptr_dispatcher_module->toString().c_str(),
                ptr_module->toString().c_str(), 
                ptr_module->toString().length()) == 0) {
        is_def_module = 1;
    }

    Array arr_args = Array::Create();
    arr_args.append(request);
    arr_args.append(response);
    arr_args.append(view);

    Variant tmp_controller = yaf_dispatcher_get_controller(app_dir.c_str(), 
            ptr_module->toString().c_str(), ptr_controller->toString().c_str(), 
            is_def_module, arr_args);
    if (tmp_controller.isNull()) {
        yaf_trigger_error(YAF_ERR_DISPATCH_FAILED, 
                "get controller failed, dir:%s", app_dir.c_str());
        return HHVM_YAF_FAILED;
    }
/*
    int retval = yaf_controller_construct(tmp_controller.toObject(), 
            request, response, view, init_null_variant);
    if (retval != HHVM_YAF_SUCCESS) {
        yaf_trigger_error(YAF_ERR_DISPATCH_FAILED, 
                "construct controller failed, dir:%s", app_dir.c_str());
        return HHVM_YAF_FAILED;
    }
    */
    char view_dir[8192];
    if (is_def_module) {
        snprintf(view_dir, sizeof(view_dir), "%s/%s", app_dir.c_str() ,"views");
    } else {
        snprintf(view_dir, sizeof(view_dir), "%s/%s/%s/%s", 
                app_dir.c_str(), "modules", ptr_module->toString().c_str(), "views");
    }

    g_yaf_local_data.get()->view_directory = std::string(view_dir);
    auto ptr_ctl_controller = tmp_controller.toObject()->o_realProp(YAF_CONTROLLER_PROPERTY_NAME_NAME,
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    *ptr_ctl_controller = *ptr_controller;

    auto ptr_action = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_ACTION, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    std::string action_lower = ptr_action->toString().toCppString();
    transform(action_lower.begin(), action_lower.end(), action_lower.begin(), tolower);

    std::string actionMethod = action_lower + "action";
    raise_warning("start check func %s of controller", actionMethod.c_str());

    Variant* executor = NULL;
    Variant var_action;
    const Func* action_func = tmp_controller.toObject()->methodNamed(String(actionMethod).get());
    if (action_func != nullptr) {
         const Func::ParamInfoVec& func_params  = action_func->params();

         Array params = Array::Create();
         String method(actionMethod);
         Array arr_func = Array::Create();

         arr_func.append(tmp_controller.toObject());
         arr_func.append(method);

         executor = &tmp_controller;
         raise_warning("parameter size:%d", func_params.size());
         if (func_params.size()) {
             yaf_dispatcher_get_call_parameters(request, action_func, params);
         } 

         Variant ret = vm_call_user_func(arr_func, params);
         if (ret.isBoolean() && !ret.toBoolean()) {
             return HHVM_YAF_SUCCESS;
         }
    } else {

        Array arr_params = Array::Create();
        arr_params.append(request);
        arr_params.append(response);
        arr_params.append(view);

        var_action = yaf_dispatcher_get_action(app_dir.c_str(), tmp_controller.toObject(), 
                             ptr_module->toString(), is_def_module, ptr_action->toString(), arr_params);
        if (var_action.isNull()) {
            raise_warning("yaf_dispatcher_get_action failed, " \
                    "module:%s action:%s", 
                    ptr_module->toString().c_str(), 
                    ptr_action->toString().c_str());
            return HHVM_YAF_FAILED;
        }

        executor = &var_action;
        const Func* action_func = var_action.toObject()->methodNamed(
                                    String(YAF_ACTION_EXECUTOR_NAME).get());
        if (action_func != nullptr) {
            yaf_controller_construct(var_action.toObject(), 
                        request, response, view, init_null_variant);

            Object o_action = var_action.toObject();
            auto ptr_name = o_action->o_realProp(YAF_CONTROLLER_PROPERTY_NAME_NAME, 
                            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
            *ptr_name = *ptr_controller;

            auto ptr_action_ctrl = o_action->o_realProp(YAF_ACTION_PROPERTY_NAME_CTRL, 
                            ObjectData::RealPropUnchecked, "Yaf_Action_Abstract");
            *ptr_action_ctrl = tmp_controller;

            const Func::ParamInfoVec& func_params  = action_func->params();

             Array params = Array::Create();
             String method(YAF_ACTION_EXECUTOR_NAME);
             Array arr_func = Array::Create();

             arr_func.append(o_action);
             arr_func.append(method);

             raise_warning("parameter size:%d", func_params.size());
             if (func_params.size()) {
                 yaf_dispatcher_get_call_parameters(request, action_func, params);
             } 

             Variant ret = vm_call_user_func(arr_func, params);
             if (ret.isBoolean() && !ret.toBoolean()) {
                 return HHVM_YAF_SUCCESS;
             }
        }
    }

    if (executor->isObject()) {
        bool auto_render = true;
        auto ptr_render = (*executor).toObject()->o_realProp(YAF_CONTROLLER_PROPERTY_NAME_RENDER,
                                ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");

        auto ptr_flush = object->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_FLUSH,
                                ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

        /*
        if (ptr_render->isNull()) {
            ptr_render = object->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_RENDER,
                                ObjectData::RealPropUnchecked, "Yaf_Dispatcher");
            if (ptr_render->isBoolean()) {
                auto_render = ptr_render->toBoolean();
            }
        } else {
            auto_render = ptr_render->toBoolean();
        }
        */
        if (executor->toObject()->o_instanceof("Yaf_Action_Abstract")) {
            raise_warning("executor is extends from Yaf_Action_Abstract");
        } 

        if (executor->toObject()->o_instanceof("Yaf_Controller_Abstract")) {
            raise_warning("executor is extends from Yaf_Controller_Abstract");
        }

        if (auto_render) {
            if (!ptr_flush->toBoolean()) {
                Array params = Array::Create();
                String method("render");
                Array arr_func = Array::Create();

                arr_func.append(executor->toObject());
                arr_func.append(method);

                params.append(ptr_action->toString());

                Variant ret = vm_call_user_func(arr_func, params);
                if (ret.isBoolean() && !ret.toBoolean()) {
                    return HHVM_YAF_FAILED;
                }

                if (ret.isString() && ret.toString().length()) {
                    raise_warning("render ret:%s", ret.toString().c_str());
                    yaf_response_alter_body(response, 
                            init_null_variant, ret.toString(), YAF_RESPONSE_APPEND);
                }
            } else {
                Array params = Array::Create();
                String method("display");
                Array arr_func = Array::Create();

                arr_func.append(*executor);
                arr_func.append(method);

                params.append(ptr_action->toString());

                Variant ret = vm_call_user_func(arr_func, params);
                if (ret.isBoolean() && !ret.toBoolean()) {
                    return HHVM_YAF_FAILED;
                }
            }
        }

    }

    return HHVM_YAF_SUCCESS;
}

static Variant yaf_dispatcher_init_view(const Object* object, 
        const Variant& tpl_dir, const Variant& options)
{
    auto ptr_view = (*object)->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_VIEW, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");
    if (ptr_view->isObject() && ptr_view->toObject()->o_instanceof("Yaf_View_Interface")) {
        return *ptr_view;
    }

    *ptr_view = yaf_view_instance(NULL, tpl_dir, options);
    return *ptr_view;
}

static int yaf_dispatcher_route(const Object* object, const Object& request)
{
    raise_warning("begin dispatcher route");
    auto ptr_router = (*object)->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_ROUTER, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");
    if (!ptr_router->isObject()) {
        raise_warning("begin dispatcher route failed, router is null");
        return HHVM_YAF_FAILED; 
    }
    
    Object o_router = ptr_router->toObject();
    String class_name = o_router->o_getClassName();
    raise_warning("get dispatcher route class_name:%s", class_name.c_str());
    if (strncasecmp(class_name.c_str(), "Yaf_Router", 
                sizeof("Yaf_Router") - 1) == 0) {
        yaf_router_route(&o_router, request);
    } else {
        Array func = Array::Create();
        Array params = Array::Create();

        func.append(o_router);
        func.append(String("route"));

        params.append(request);

        Variant ret = vm_call_user_func(func, params);
        if (ret.isBoolean() && ret.toBoolean() == false) {
            yaf_trigger_error(YAF_ERR_ROUTE_FAILED, "routing request failed");
            raise_warning("call dispatcher route failed, router is null");
            return HHVM_YAF_FAILED;
        }
    }

    return HHVM_YAF_SUCCESS;
}

static int yaf_dispatcher_fix_default(const Object* object, const Object& request)
{
    auto ptr_module = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_MODULE, 
                                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    auto ptr_action = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_ACTION, 
                                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    auto ptr_controller = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_CONTROLLER, 
                                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    if (!ptr_module->isString() || ptr_module->toString().length() == 0) {

        auto ptr_def_module = (*object)->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_MODULE, 
                                ObjectData::RealPropUnchecked, "Yaf_Dispatcher");
        *ptr_module = *ptr_def_module;
    } else {
        std::string str_module = ptr_module->toString().toCppString();
        transform(str_module.begin(), str_module.end(), str_module.begin(), tolower);
        str_module[0] = toupper(str_module[0]);

        *ptr_module = String(str_module);
    }

    if (!ptr_controller->isString() || ptr_controller->toString().length() == 0) {
        auto ptr_def_controller = (*object)->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_CONTROLLER, 
                                ObjectData::RealPropUnchecked, "Yaf_Dispatcher");
        *ptr_controller = *ptr_def_controller;
    } else {
        
        std::string str_controller = ptr_controller->toString().toCppString();
        transform(str_controller.begin(), str_controller.end(), str_controller.begin(), tolower);
        str_controller[0] = toupper(str_controller[0]);

        /**
         * upper controller name
         * eg: Index_sub -> Index_Sub
         */
        for (int i = 0; i < str_controller.length(); i++) {
            if (str_controller[i] == '_'
#ifdef YAF_HAVE_NAMESPACE
                    || str_controller[i] == '\\'
#endif
               ) {
                if (i + 1 != str_controller.length() - 1) {
                    str_controller[i+1] = toupper(str_controller[i+1]);
                }
            }
        }

        *ptr_controller = String(str_controller);
    }

    if (!ptr_action->isString() || ptr_action->toString().length() == 0) {
        auto ptr_def_action = (*object)->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_ACTION, 
                                ObjectData::RealPropUnchecked, "Yaf_Dispatcher");
        *ptr_action = *ptr_def_action;
    } else {
        std::string str_action = ptr_action->toString().toCppString();
        transform(str_action.begin(), str_action.end(), str_action.begin(), tolower);
        
        *ptr_action = String(str_action);
    }

    raise_warning("module:%s ctrl:%s action:%s",
            ptr_module->toString().c_str(),
            ptr_controller->toString().c_str(),
            ptr_action->toString().c_str());

    return HHVM_YAF_SUCCESS;
}

static void yaf_dispatcher_call_router_hook(const Array& arr_plugin, 
        const Object& request, const Object& response, String function)
{
    ArrayIter iter = arr_plugin.begin();
    while (!iter.end()) {
        Variant value = iter.second();
        if (!value.isObject()) {
            iter.next();
            continue;
        }
        
        Array func = Array::Create();
        Array params = Array::Create();

        func.append(value.toObject());
        func.append(function);

        params.append(request);
        params.append(response);

        vm_call_user_func(func, params);
        iter.next();
    }
}

Variant yaf_dispatcher_dispatch(const Object* object)
{
    //TODO need modify sapi_name to support cli or http mode
    Variant var_response = yaf_response_instance(NULL, "online");
    auto ptr_request = (*object)->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_REQUEST, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    auto ptr_plugin = (*object)->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_PLUGINS, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    if (!ptr_plugin->isArray()) {
        *ptr_plugin = Array::Create();
    }

    if (ptr_request->isObject() == false) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, "Expect a object of request");
        return false;
    }

    Object response = var_response.toObject();
    Object request = ptr_request->toObject();
    Array& arr_plugin = ptr_plugin->toArrRef();

    raise_warning("begin request route");
    if (!yaf_request_is_routed(&request)) {

        raise_warning("in request route");
        yaf_dispatcher_call_router_hook(arr_plugin, request, response, 
                String(YAF_PLUGIN_HOOK_ROUTESTARTUP));

        //TODO need implement YAF_EXCEPTION_HANDLE
        //YAF_EXCEPTION_HANDLE(dispatcher, request, response); 
        if (yaf_dispatcher_route(object, request) != HHVM_YAF_SUCCESS) {
            yaf_trigger_error(YAF_ERR_ROUTE_FAILED, "Routing request failed");
            //TODO need implement
            //YAF_EXCEPTION_HANDLE_NORET
            return init_null_variant;
        }

        yaf_dispatcher_fix_default(object, request);
        yaf_dispatcher_call_router_hook(arr_plugin, request, response, 
                String(YAF_PLUGIN_HOOK_ROUTESHUTDOWN));

        //TODO need implement YAF_EXCEPTION_HANDLE
        //YAF_EXCEPTION_HANDLE(dispatcher, request, response); 
        yaf_request_set_routed(&request, 1);
    } else {
        yaf_dispatcher_fix_default(object, request);
    }

    yaf_dispatcher_call_router_hook(arr_plugin, request, response, 
            String(YAF_PLUGIN_HOOK_LOOPSTARTUP));

    //TODO need implement YAF_EXCEPTION_HANDLE
    //YAF_EXCEPTION_HANDLE(dispatcher, request, response); 
   
    Variant view = yaf_dispatcher_init_view(object, init_null_variant, init_null_variant);
    if (view.isNull()) {
        return init_null_variant;
    }

    int nesting = g_yaf_local_data.get()->forward_limit;

    do {
        yaf_dispatcher_call_router_hook(arr_plugin, request, response, 
                String(YAF_PLUGIN_HOOK_PREDISPATCH));
        if (yaf_dispatcher_handle(*object, request, response, view.toObject()) != HHVM_YAF_SUCCESS) {
            //YAF_EXCEPTION_HANDLE(dispatcher, request, response);
            return init_null_variant;
        }

        yaf_dispatcher_fix_default(object, request);
        yaf_dispatcher_call_router_hook(arr_plugin, request, response, 
                String(YAF_PLUGIN_HOOK_POSTDISPATCH));

        //TODO need implement YAF_EXCEPTION_HANDLE
        //YAF_EXCEPTION_HANDLE(dispatcher, request, response); 
    } while (--nesting > 0 && !yaf_request_is_dispatched(&request));

    yaf_dispatcher_call_router_hook(arr_plugin, request, response, 
            String(YAF_PLUGIN_HOOK_LOOPSHUTDOWN));

    //TODO need implement YAF_EXCEPTION_HANDLE
    //YAF_EXCEPTION_HANDLE(dispatcher, request, response); 
    if (nesting == 0 && !yaf_request_is_dispatched(&request)) {
        yaf_trigger_error(YAF_ERR_DISPATCH_FAILED, 
                "The max dispatch nesting %ld was reached",
                g_yaf_local_data.get()->forward_limit);

        return init_null_variant;
    }

    auto ptr_return_response = (*object)->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_RETURN, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    if (!ptr_return_response->toBoolean()) {
        raise_warning("begin send to client");
        (void)yaf_response_send(response);
        yaf_response_clear_body(&response, init_null_variant);
    }

    raise_warning("handle request finished, return it");
    return response;
}

int  yaf_dispatcher_set_request(const Object* object, const Variant& request)
{
    auto ptr_request = (*object)->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_REQUEST, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");
    *ptr_request = request.toObject();

    return HHVM_YAF_SUCCESS;
}


static void HHVM_METHOD(Yaf_Dispatcher, __clone)
{
}

static void HHVM_METHOD(Yaf_Dispatcher, __construct)
{
}

static void HHVM_METHOD(Yaf_Dispatcher, __sleep)
{
}

static void HHVM_METHOD(Yaf_Dispatcher, __wakeup)
{
}

static Variant HHVM_METHOD(Yaf_Dispatcher, enableView)
{
    auto ptr_render = this_->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_RENDER, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    *ptr_render = Variant(true);
    return this_;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, disableView)
{
    auto ptr_render = this_->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_RENDER, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    *ptr_render = Variant(false);
    return this_;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, initView, const Variant& tpl_dir, 
        const Variant& options)
{
    Variant view = yaf_dispatcher_init_view(&this_, tpl_dir, options);
    if (!view.isNull()) {
        return view;
    }

    return false;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, setView, const Variant& view) 
{
    if (view.isObject() && view.toObject()->o_instanceof("Yaf_View_Interface")) {
        auto ptr_view = this_->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_VIEW, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");
        *ptr_view = view;
        return this_;
    }

    return false;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, setRequest, const Variant& request) 
{
    if (!request.isObject() || !request.toObject()->o_instanceof("Yaf_Request_Abstract")) {
        yaf_trigger_error(YAF_ERR_TYPE_WARN, 
                "Expect request to extend from Yaf_Request_Abstract");
        return false;
    }

    int ret = yaf_dispatcher_set_request(&this_, request);
    if (ret != HHVM_YAF_SUCCESS) {
        return false;
    }

    return this_;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, getApplication) 
{
    return get_app();
}

static Variant HHVM_METHOD(Yaf_Dispatcher, getRouter) 
{
    auto ptr_route = this_->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_ROUTER, 
        ObjectData::RealPropUnchecked, "Yaf_Dispatcher");
    return *ptr_route;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, getRequest) 
{
    auto ptr_req = this_->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_REQUEST, 
        ObjectData::RealPropUnchecked, "Yaf_Dispatcher");
    return *ptr_req;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, setErrorHandler,
        const String& callback, const Variant& type) 
{
    Array params = Array::Create();
    params.append(callback);
    params.append(type);
    vm_call_user_func(String("set_error_handler"), params);

    return this_;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, setDefaultModule, const Variant& module) 
{
    if (!module.isString()) {
        return false;
    }

    if (module.toString().length() == 0) {
        return false;
    }

    String str_module = module.toString();
    bool ret = yaf_application_is_module_name(str_module);
    if (ret == false) {
        return false;
    }

    auto ptr_module = this_->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_MODULE, 
        ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    std::string tmp = str_module.toCppString();
    transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
    tmp[0] = toupper(tmp[0]);

    *ptr_module = String(tmp);
    return this_;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, setDefaultController, const Variant& controller) 
{
    if (!controller.isString()) {
        return false;
    }

    if (controller.toString().length() == 0) {
        return false;
    }

    String str_controller = controller.toString();

    auto ptr_controller = this_->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_CONTROLLER, 
        ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    std::string tmp = str_controller.toCppString();
    transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
    tmp[0] = toupper(tmp[0]);

    *ptr_controller = String(tmp);
    return this_;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, setDefaultAction, const Variant& action) 
{
    if (!action.isString()) {
        return false;
    }

    if (action.toString().length() == 0) {
        return false;
    }

    String str_action = action.toString();

    auto ptr_action = this_->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_ACTION, 
        ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    std::string tmp = str_action.toCppString();
    transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
    tmp[0] = toupper(tmp[0]);

    *ptr_action = String(tmp);
    return this_;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, returnResponse, const Variant& res) 
{
    auto ptr_return = this_->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_RETURN, 
        ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    if (!res.isNull()) {
        *ptr_return = res;
        return this_;
    }

    return *ptr_return;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, autoRender, const Variant& render) 
{
    auto ptr_render = this_->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_RENDER, 
        ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    if (!render.isNull()) {
        *ptr_render = render;
        return this_;
    }

    return *ptr_render;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, flushInstantly, const Variant& flush) 
{
    auto ptr_flush = this_->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_FLUSH, 
        ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    if (!flush.isNull()) {
        *ptr_flush = flush;
        return this_;
    }

    return *ptr_flush;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, dispatch, const Variant& request) 
{
    auto ptr_req = this_->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_REQUEST, 
        ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    *ptr_req = request;
    Variant response = yaf_dispatcher_dispatch(&this_);
    return response;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, throwException, const Variant& flag) 
{
    if (flag.isBoolean()) {
        bool throw_exception = flag.toBoolean();
        g_yaf_local_data.get()->throw_exception =  throw_exception;
        return this_;
    } 

    return g_yaf_local_data.get()->throw_exception;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, catchException, const Variant& flag) 
{
    if (flag.isBoolean()) {
        g_yaf_local_data.get()->catch_exception = flag.toBoolean();
        return this_;
    } 

    return g_yaf_local_data.get()->catch_exception;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, registerPlugin, const Variant& plugin) 
{
    if (!plugin.isObject()) {
        raise_warning("Expect a instance of Yaf_Plugin_Abstract");
        return false;
    }

    Object o_plugin = plugin.toObject();
    if (!o_plugin->o_instanceof(String("Yaf_Plugin_Abstract"))) {
        raise_warning("Expect a instance of Yaf_Plugin_Abstract");
        return false;
    }

    auto ptr_plugin = this_->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_PLUGINS, 
        ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    if (!ptr_plugin->isArray()) {
        *ptr_plugin = Array::Create();
    }

    Array& arr_plugin = ptr_plugin->toArrRef();
    arr_plugin.append(o_plugin);

    return this_;
}

void YafExtension::_initYafDispatcherClass()
{
    HHVM_ME(Yaf_Dispatcher, __clone);
    HHVM_ME(Yaf_Dispatcher, __construct);
    HHVM_ME(Yaf_Dispatcher, __sleep);
    HHVM_ME(Yaf_Dispatcher, __wakeup);

    HHVM_ME(Yaf_Dispatcher, enableView);
    HHVM_ME(Yaf_Dispatcher, disableView);
    HHVM_ME(Yaf_Dispatcher, initView);
    HHVM_ME(Yaf_Dispatcher, setView);
    HHVM_ME(Yaf_Dispatcher, setRequest);
    HHVM_ME(Yaf_Dispatcher, getApplication);
    HHVM_ME(Yaf_Dispatcher, getRouter);
    HHVM_ME(Yaf_Dispatcher, getRequest);

    HHVM_ME(Yaf_Dispatcher, setErrorHandler);
    HHVM_ME(Yaf_Dispatcher, setDefaultModule);
    HHVM_ME(Yaf_Dispatcher, setDefaultController);
    HHVM_ME(Yaf_Dispatcher, setDefaultAction);
    HHVM_ME(Yaf_Dispatcher, returnResponse);

    HHVM_ME(Yaf_Dispatcher, autoRender);
    HHVM_ME(Yaf_Dispatcher, flushInstantly);
    HHVM_ME(Yaf_Dispatcher, dispatch);

    HHVM_ME(Yaf_Dispatcher, throwException);
    HHVM_ME(Yaf_Dispatcher, catchException);
    HHVM_ME(Yaf_Dispatcher, registerPlugin);
}


}
