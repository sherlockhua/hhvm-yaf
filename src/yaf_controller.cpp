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
//#include "ext_example.h"

#include <iostream>  
#include <algorithm> 

#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/ext/extension.h"
#include "ext_yaf.h"
#include "yaf_request.h"
#include "yaf_response.h"
#include "yaf_controller.h"

namespace HPHP { 

int yaf_controller_construct(Variant& control, Object& request, 
        Object& response, Object& view, const Variant& params)
{
    return HHVM_YAF_SUCCESS;
}

static Variant HHVM_METHOD(Yaf_Controller_Abstract, test)
{
    return String("this is in cpp test!!!");
}

static Variant HHVM_METHOD(Yaf_Controller_Abstract, render, const String& tpl, 
        const Variant& parameters)
{
    auto _viewTmp = this_->o_realProp("_view", 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    if (_viewTmp->isNull()) {
        return false;
    }

    auto _view = _viewTmp->toObject();
    std::string self_name = this_->o_realProp("_name", 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract")->toString().toCppString();

    std::string view_ext = g_yaf_local_data.get()->view_ext;
    transform(self_name.begin(), self_name.end(), self_name.begin(), tolower);

    for (unsigned int i = 0; i < self_name.length(); i++) {
        if (self_name[i] == '_') {
            self_name[i] = DEFAULT_SLASH_CHAR;
        }
    }

    std::string action_name = tpl.toCppString();

    for (unsigned int i = 0; i < action_name.length(); i++) {
        if (action_name[i] == '_') {
            action_name[i] = DEFAULT_SLASH_CHAR;
        }
    }

    std::string path = self_name  + DEFAULT_SLASH_STR + action_name + 
                        "." + view_ext;

    Array func = Array::Create();
    func.append(_view);
    func.append(String("render"));

    Array params = Array::Create();
    params.append(String(path.c_str()));

    if (!parameters.isNull()) {
        params.append(parameters);
    }

    Variant ret = vm_call_user_func(func, params);
    if (ret.isNull()) {
        return false;
    }

    if (ret.isBoolean() && !ret.toBoolean()) {
        return false;
    }

    return ret;
}

static Variant HHVM_METHOD(Yaf_Controller_Abstract, display, const String& tpl, 
        const Variant& parameters)
{
    auto _viewTmp = this_->o_realProp("_view", 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    if (_viewTmp->isNull()) {
        return false;
    }

    auto _view = _viewTmp->toObject();
    std::string self_name = this_->o_realProp("_name", 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract")->toString().toCppString();

    std::string view_ext = g_yaf_local_data.get()->view_ext;
    transform(self_name.begin(), self_name.end(), self_name.begin(), tolower);

    for (unsigned int i = 0; i < self_name.length(); i++) {
        if (self_name[i] == '_') {
            self_name[i] = DEFAULT_SLASH_CHAR;
        }
    }

    std::string action_name = tpl.toCppString();

    for (unsigned int i = 0; i < action_name.length(); i++) {
        if (action_name[i] == '_') {
            action_name[i] = DEFAULT_SLASH_CHAR;
        }
    }

    std::string path = self_name  + DEFAULT_SLASH_STR + action_name + 
                        "." + view_ext;

    Array func = Array::Create();
    func.append(_view);
    func.append(String("display"));

    Array params = Array::Create();
    params.append(String(path.c_str()));

    if (!parameters.isNull()) {
        params.append(parameters);
    }

    Variant ret = vm_call_user_func(func, params);
    if (ret.isNull()) {
        return false;
    }

    if (ret.isBoolean() && !ret.toBoolean()) {
        return false;
    }

    return ret;
}

static Variant HHVM_METHOD(Yaf_Controller_Abstract, getRequest)
{
    auto _requestTmp = this_->o_realProp("_request", 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    if (_requestTmp->isNull()) {
        return *_requestTmp;
    }

    return _requestTmp->toObject();
}

static Variant HHVM_METHOD(Yaf_Controller_Abstract, getResponse)
{
    auto _responseTmp = this_->o_realProp("_response", 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    if (_responseTmp->isNull()) {
        return *_responseTmp;
    }

    return _responseTmp->toObject();
}

static Variant HHVM_METHOD(Yaf_Controller_Abstract, getModuleName)
{
    auto tmp = this_->o_realProp("_module", 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    if (tmp->isNull()) {
        return *tmp;
    }

    return tmp->toObject();
}

static Variant HHVM_METHOD(Yaf_Controller_Abstract, getView)
{
    auto tmp = this_->o_realProp("_view", 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    if (tmp->isNull()) {
        return *tmp;
    }

    return tmp->toObject();
}

static Variant HHVM_METHOD(Yaf_Controller_Abstract, initView, const Variant& options)
{
    return this_;
}

static bool HHVM_METHOD(Yaf_Controller_Abstract, setViewpath, 
        const String& path)
{
    auto _viewTmp = this_->o_realProp("_view", 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    if (_viewTmp->isNull()) {
        return false;
    }

    auto _view = _viewTmp->toObject();
    if (_view->o_instanceof(String("Yaf_View_Simple"))) {

        auto tpl_dir = _view->o_realProp("_tpl_dir", 
            ObjectData::RealPropUnchecked, "Yaf_View_Simple");
        *tpl_dir = path;
    } else {
        Array func = Array::Create();
        func.append(_view);
        func.append(String("setscriptpath"));

        Array params = Array::Create();
        params.append(path);

        vm_call_user_func(func, params);
    }

    return true;
}

static Variant HHVM_METHOD(Yaf_Controller_Abstract, getViewpath, 
        const String& path)
{
    auto _viewTmp = this_->o_realProp("_view", 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    if (_viewTmp->isNull()) {
        return false;
    }

    Variant output;
    auto _view = _viewTmp->toObject();
    if (_view->o_instanceof(String("Yaf_View_Simple"))) {

        auto tpl_dir = _view->o_realProp("_tpl_dir", 
            ObjectData::RealPropUnchecked, "Yaf_View_Simple");
        if (!tpl_dir->isString()) {
            std::string view_dir = g_yaf_local_data.get()->view_directory;
            return Variant(String(view_dir.c_str()));
        }

        return tpl_dir->toString();
    } else {
        Array func = Array::Create();
        func.append(_view);
        func.append(String("getscriptpath"));

        Array params = Array::Create();
        output = vm_call_user_func(func, params);
    }

    return output;
}

static Variant HHVM_METHOD(Yaf_Controller_Abstract, forward,  
        const Variant& module, const Variant& controller, 
        const Variant& action, const Variant& parameters)
{
    auto tmp = this_->o_realProp("_request", 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    if (tmp->isNull()) {
        return false;
    }

    auto request = tmp->toObject();
    if (!request->o_instanceof(String("Yaf_Request_Abstract"))) {
        return false;
    }
    
    if (!module.isString()) {
        raise_error("Expect a string action name");
        return false;
    }

    if (parameters.isArray()) {
        auto var_params = request->o_realProp("params", 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_params = parameters;


        auto var_module = request->o_realProp("module", 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_module = module;

        auto var_controller = request->o_realProp("controller", 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_controller = controller;

        auto var_action = request->o_realProp("action", 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_action = action;
    } else if (!parameters.isNull()){
        raise_error("parameters must be array");
        return false;
    } else if (action.isArray()) {
        auto var_params = request->o_realProp("params", 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_params = action;

        auto var_controller = request->o_realProp("controller", 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_controller = module;

        auto var_action = request->o_realProp("action", 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_action = controller;
    } else if (action.isString()) {
         auto var_module = request->o_realProp("module", 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_module = module;

        auto var_controller = request->o_realProp("controller", 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_controller = controller;

        auto var_action = request->o_realProp("action", 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_action = action;
    } else if (!action.isNull()) {
        return false;
    } else if (controller.isArray()) {
        auto var_params = request->o_realProp("params", 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_params = controller;

        auto var_action = request->o_realProp("action", 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_action = module;
    } else if (controller.isString()) {
        auto var_controller = request->o_realProp("controller", 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_controller = module;

        auto var_action = request->o_realProp("action", 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_action = controller;
    } else if (!controller.isNull()) {
        return false;
    } else if (module.isString()){
        auto var_action = request->o_realProp("action", 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_action = module;
    } else {
        raise_error("module must be string");
        return false;
    }

    yaf_request_set_dispatched(request, 0);
    return true;
}

static Variant HHVM_METHOD(Yaf_Controller_Abstract, redirect,  
        const String& location)
{
    auto tmp = this_->o_realProp(YAF_CONTROLLER_PROPERTY_NAME_RESPONSE, 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    if (tmp->isNull()) {
        return false;
    }

    auto response = tmp->toObject();
    (void)yaf_response_set_redirect(response, location);
    return true;
}

static Variant HHVM_METHOD(Yaf_Controller_Abstract, getInvokeArgs)
{
    auto tmp = this_->o_realProp(YAF_CONTROLLER_PROPERTY_NAME_ARGS, 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    return *tmp;
}

static Variant HHVM_METHOD(Yaf_Controller_Abstract, getInvokeArg, const String& name)
{
    if (name.length() == 0) {
        return Variant(Variant::NullInit());
    }

    auto tmp = this_->o_realProp(YAF_CONTROLLER_PROPERTY_NAME_ARGS, 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    if (tmp->isNull()) {
        return Variant(Variant::NullInit());
    }

    if (tmp->isArray()) {
        auto array = tmp->toArray();
        if (array.exists(name)) {
            return array[name];
        }
    }

    return Variant(Variant::NullInit());
}

static void HHVM_METHOD(Yaf_Controller_Abstract, __construct, 
        const Object& request, const Object& response,
        const Object& view, const Variant& invokeArgs)
{
    if (!invokeArgs.isNull()) {
        auto var_params = this_->o_realProp(YAF_CONTROLLER_PROPERTY_NAME_ARGS, 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *var_params = invokeArgs;
    }

    auto var_request = this_->o_realProp(YAF_CONTROLLER_PROPERTY_NAME_REQUEST, 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    *var_request = request;

    auto var_response = this_->o_realProp(YAF_CONTROLLER_PROPERTY_NAME_RESPONSE, 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    *var_response = response;

    auto var_view = this_->o_realProp(YAF_CONTROLLER_PROPERTY_NAME_VIEW, 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    *var_view = view;

    auto module = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_MODULE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    auto var_module = this_->o_realProp(YAF_CONTROLLER_PROPERTY_NAME_REQUEST, 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    *var_module = *module;

    if (!this_->o_instanceof("Yaf_Action_Abstract")) {
        Array func = Array::Create();
        func.append(this_);
        func.append(String("init"));

        Array params = Array::Create();
        vm_call_user_func(func, params);
    }
}


static void HHVM_METHOD(Yaf_Controller_Abstract, __clone)
{}

void YafExtension::_initYafControllerClass()
{
    HHVM_ME(Yaf_Controller_Abstract, render);
    HHVM_ME(Yaf_Controller_Abstract, display);
    HHVM_ME(Yaf_Controller_Abstract, getRequest);
    HHVM_ME(Yaf_Controller_Abstract, getResponse);
    HHVM_ME(Yaf_Controller_Abstract, getModuleName);
    HHVM_ME(Yaf_Controller_Abstract, getView);
    HHVM_ME(Yaf_Controller_Abstract, initView);
    HHVM_ME(Yaf_Controller_Abstract, setViewpath);
    HHVM_ME(Yaf_Controller_Abstract, getViewpath);
    HHVM_ME(Yaf_Controller_Abstract, forward);
    HHVM_ME(Yaf_Controller_Abstract, redirect);
    HHVM_ME(Yaf_Controller_Abstract, getInvokeArgs);
    HHVM_ME(Yaf_Controller_Abstract, getInvokeArg);
    HHVM_ME(Yaf_Controller_Abstract, __construct);
    HHVM_ME(Yaf_Controller_Abstract, __clone);
    HHVM_ME(Yaf_Controller_Abstract, test);
}

}
