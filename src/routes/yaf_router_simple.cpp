/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-02 12:05:29
*   description  ：
*
=============================================*/
#include "yaf_router_simple.h"
#include "hphp/runtime/base/php-globals.h"
#include "yaf_application.h"
#include "ext_yaf.h"
#include "yaf_request.h"


namespace HPHP {


static Variant yaf_route_simple_instance(const Object* object, 
        const String& module, const String& controller, const String& action)
{
    Object o;
    if (object != NULL) {
        o = *object;
    } else {
        Array params = Array::Create();
        params.append(module);
        params.append(controller);
        params.append(action);

        o = createObject("Yaf_Route_Simple", params);
    }

    auto ptr_module = o->o_realProp(YAF_ROUTE_SIMPLE_VAR_NAME_MODULE, 
                ObjectData::RealPropUnchecked, "Yaf_Route_Simple");

    auto ptr_controller = o->o_realProp(YAF_ROUTE_SIMPLE_VAR_NAME_CONTROLLER, 
                ObjectData::RealPropUnchecked, "Yaf_Route_Simple");

    auto ptr_action = o->o_realProp(YAF_ROUTE_SIMPLE_VAR_NAME_ACTION, 
                ObjectData::RealPropUnchecked, "Yaf_Route_Simple");

    *ptr_module = module;
    *ptr_controller = controller;
    *ptr_action = action;

    return o;
}

static Variant yaf_route_simple_route(const Object& o, const Object& request)
{
    auto ptr_module = o->o_realProp(YAF_ROUTE_SIMPLE_VAR_NAME_MODULE, 
                ObjectData::RealPropUnchecked, "Yaf_Route_Simple");

    auto ptr_controller = o->o_realProp(YAF_ROUTE_SIMPLE_VAR_NAME_CONTROLLER, 
                ObjectData::RealPropUnchecked, "Yaf_Route_Simple");

    auto ptr_action = o->o_realProp(YAF_ROUTE_SIMPLE_VAR_NAME_ACTION, 
                ObjectData::RealPropUnchecked, "Yaf_Route_Simple");

    Variant module;
    Variant controller;
    Variant action;

    if (php_global(S_GET).toArray().exists(ptr_module->toString())) {
        module = php_global(S_GET).toArray()[ptr_module->toString()];
    }

    if (php_global(S_GET).toArray().exists(ptr_controller->toString())) {
        controller = php_global(S_GET).toArray()[ptr_controller->toString()];
    }

    if (php_global(S_GET).toArray().exists(ptr_action->toString())) {
        action = php_global(S_GET).toArray()[ptr_action->toString()];
    }

    if (module.isNull() && controller.isNull() && action.isNull()) {
        return false;
    }

    if (module.isString()) {
        if (yaf_application_is_module_name(module.toString())) {
            auto ptr_req_module = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_MODULE, 
                        ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
            *ptr_req_module = module;
        }
    }

    auto ptr_req_controller = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_CONTROLLER, 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    auto ptr_req_action = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_ACTION, 
                ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    *ptr_req_controller = controller;
    *ptr_req_action = action;

    return true;
}

static void HHVM_METHOD(Yaf_Route_Simple, __construct, 
        const Variant& module, const Variant& controller, 
        const Variant& action)
{
    if (!module.isString() || !controller.isString() || !action.isString()) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                "Yaf_Route_Simple, Expect 3 string paramsters");
        return;
    }

    (void)yaf_route_simple_instance(&this_, module.toString(), 
            controller.toString(), action.toString());
}

static Variant HHVM_METHOD(Yaf_Route_Simple, route, const Variant& request)
{
    if (!request.isObject()) {
        yaf_trigger_error(YAF_ERR_ROUTE_FAILED, 
                "trigger error failed, request is not object");
        return false;
    }

    return yaf_route_simple_route(this_, request.toObject());
}

void YafExtension::_initYafSimpleRouterClass()
{
    HHVM_ME(Yaf_Route_Simple, __construct);
    HHVM_ME(Yaf_Route_Simple, route);
}

}
