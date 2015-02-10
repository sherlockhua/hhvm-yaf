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

namespace HPHP {


Variant yaf_dispatcher_dispatch(const Object* object)
{
    //TODO need modify sapi_name to support cli or http mode
    Variant response = yaf_response_instance(NULL, "online");
    auto ptr_request = (*object)->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_REQUEST, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    auto ptr_plugin = (*object)->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_PLUGINS, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    if (ptr_request->isObject() == false) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, "Expect a object of request");
        return false;
    }

    Object request = ptr_request->toObject();
    if (!yaf_request_is_routed(&request)) {
        
    } else {

    }

    return init_null_variant;
}

int  yaf_dispatcher_set_request(const Object* object, const Variant& request)
{
    auto ptr_request = (*object)->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_REQUEST, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");
    *ptr_request = request;

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
}


}
