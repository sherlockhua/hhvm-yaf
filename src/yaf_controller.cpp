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

namespace HPHP { 


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
    HHVM_ME(Yaf_Controller_Abstract, test);
}

}
