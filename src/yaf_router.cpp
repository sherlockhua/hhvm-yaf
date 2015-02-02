/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-01 20:20:59
*   description  ：
*
=============================================*/
#include "yaf_router.h"
#include "ext_yaf.h"
#include "error.h"
#include "yaf_config.h"
#include "yaf_request.h"
#include "routes/yaf_router_interface.h"


namespace HPHP {


static int yaf_router_route (const Object* object, const Variant& request)
{
    auto ptr_routes = (*object)->o_realProp(YAF_ROUTER_PROPERTY_NAME_ROUTERS, 
            ObjectData::RealPropUnchecked, "Yaf_Router");
    if (!ptr_routes->isArray()) {
        return HHVM_YAF_FAILED;
    }

    Array& arr_routes = ptr_routes->toArrRef();
    ArrayIter iter = arr_routes.begin();
    while (!iter.end()) {
        Variant key = iter.first();
        Variant value = iter.second();

        Object route = value.toObject();

        Array func = Array::Create();
        func.append(route);
        func.append(String("route"));

        Array params = Array::Create();
        params.append(request);

        Variant ret = vm_call_user_func(func, params);
        if (!ret.isBoolean() || ret.toBoolean() == false) {
            iter.next();
            continue;
        }

        auto ptr_routes = (*object)->o_realProp(YAF_ROUTER_PROPERTY_NAME_CURRENT_ROUTE, 
                ObjectData::RealPropUnchecked, "Yaf_Router");
        *ptr_routes = key;

        Object o_request = request.toObject();
        yaf_request_set_routed(&o_request, 1);
        
        iter.next();
    }

    return HHVM_YAF_SUCCESS;
}

static int yaf_router_add_config(const Object* object, const Array& routes)
{
    if (object == NULL) {
        return HHVM_YAF_FAILED;
    }

    auto ptr_routes = (*object)->o_realProp(YAF_ROUTER_PROPERTY_NAME_ROUTERS, 
            ObjectData::RealPropUnchecked, "Yaf_Router");

    Array& arr_routes = ptr_routes->toArrRef();
    ArrayIter iter = routes.begin();
    while (!iter.end()) {
        Variant key = iter.first();
        Variant value = iter.second();
        if (!value.isArray()) {
            iter.next();
            continue;
        }

        Variant tmp = yaf_route_instance(NULL, value);
        if (tmp.isNull()) {
            iter.next();
            continue;
        }

        if (key.isString()) {
            arr_routes.set(key.toString(), tmp.toObject());
        } else if(key.isInteger()) {
            arr_routes.set(key, tmp.toObject());
        } else {
            iter.next();
            continue;
        }

        iter.next();
    }

    return HHVM_YAF_SUCCESS;
}

Variant yaf_router_instance(const Object* object)
{
    Object o;
    if (object == NULL) {
        Array arr = Array::Create();
        o = createObject("Yaf_Router", arr) ;
    } else {
        o = *object;
    }

    Object route;

    Array routes = Array::Create();
    if (!g_yaf_local_data.get()->default_route.isInitialized()) {
static_route:
        Array arr = Array::Create();
        route = createObject("Yaf_Route_Static", arr);
    } else {
        Variant tmp = yaf_route_instance(NULL, 
                g_yaf_local_data.get()->default_route);
        if (tmp.isNull()) {
            //TODO need more info about route name
            raise_warning("Unable to initialize default route"\
                    ", use other instead");
           goto static_route; 
        }
    }

    routes.set(String("_default"), route);

    auto ptr_routes = o->o_realProp(YAF_ROUTER_PROPERTY_NAME_ROUTERS, 
            ObjectData::RealPropUnchecked, "Yaf_Router");
    *ptr_routes = routes;

    return o;
} 

static void HHVM_METHOD(Yaf_Router, __construct)
{
    (void)yaf_router_instance(&this_);
}

static Variant HHVM_METHOD(Yaf_Router, addRoute, 
        const String& name, const Variant& route)
{
    if (!name.length()) {
        return false;
    }

    if (!route.isObject()) {
        yaf_trigger_error(YAF_ERR_ROUTE_FAILED, 
                "Expect route to be a object");
        return false;
    }

    Object o = route.toObject();
    if (!o->o_instanceof("Yaf_Route")) {
        yaf_trigger_error(YAF_ERR_ROUTE_FAILED, 
                "Expect route extends from Yaf_Route");
        return false;
    }

    auto ptr_routes = o->o_realProp(YAF_ROUTER_PROPERTY_NAME_ROUTERS, 
            ObjectData::RealPropUnchecked, "Yaf_Router");

    if (!ptr_routes->isArray()) {
        *ptr_routes = Array::Create();
    }

    Array& arr_routes = ptr_routes->toArrRef();
    arr_routes.set(name, o);

    return this_;
}

static Variant HHVM_METHOD(Yaf_Router, addConfig, 
        const Variant& config)
{
    if (config.isArray() == false && config.isObject() == false){
        yaf_trigger_error(YAF_ERR_ROUTE_FAILED, 
                "Expect config to be Array or Object");
        return false;
    }

    Array route;
    if (config.isArray()) {
        route = config;
    } else {
        Object o = config.toObject();
        if (!o->o_instanceof("Yaf_Config_Abstract")) {
            yaf_trigger_error(YAF_ERR_ROUTE_FAILED, 
                "Expect config extends from Yaf_Config_Abstract");
            return false;
        }

        auto ptr_config = o->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Abstract");
        if (ptr_config->isArray()) {
            route = ptr_config->toArray();
        }
    }

    int ret = yaf_router_add_config(&this_, route);
    if (ret != HHVM_YAF_SUCCESS) {
        yaf_trigger_error(YAF_ERR_ROUTE_FAILED, 
            "yaf_router_add_config failed, ret:%d", ret);
        return false;
    }

    return this_;
}

static Variant HHVM_METHOD(Yaf_Router, route, 
        const Variant& request)
{
    int ret = yaf_router_route(&this_, request);
    if (ret != HHVM_YAF_SUCCESS) {
        return false;
    }

    return true;
}

static Variant HHVM_METHOD(Yaf_Router, getRoute, 
        const String& name)
{
    auto ptr_routes = this_->o_realProp(YAF_ROUTER_PROPERTY_NAME_ROUTERS, 
            ObjectData::RealPropUnchecked, "Yaf_Router");
    if (!ptr_routes->isArray()) {
        return init_null_variant;
    }

    Array& arr_routes = ptr_routes->toArrRef();
    if (!arr_routes.exists(name)) {
        return init_null_variant;
    }

    return arr_routes[name];
}

static Variant HHVM_METHOD(Yaf_Router, getRoutes, 
        const String& name)
{
    auto ptr_routes = this_->o_realProp(YAF_ROUTER_PROPERTY_NAME_ROUTERS, 
            ObjectData::RealPropUnchecked, "Yaf_Router");
    return *ptr_routes;
}

static Variant HHVM_METHOD(Yaf_Router, getCurrentRoute)
{
    auto ptr_routes = this_->o_realProp(YAF_ROUTER_PROPERTY_NAME_CURRENT_ROUTE, 
            ObjectData::RealPropUnchecked, "Yaf_Router");
    return *ptr_routes;
}

void YafExtension::_initYafRouterClass()
{
    HHVM_ME(Yaf_Router, __construct);
    HHVM_ME(Yaf_Router, addRoute);
    HHVM_ME(Yaf_Router, addConfig);
    HHVM_ME(Yaf_Router, route);
    HHVM_ME(Yaf_Router, getRoute);
    HHVM_ME(Yaf_Router, getRoutes);
    HHVM_ME(Yaf_Router, getCurrentRoute);

    _initYafMapRouterClass();
}
 
}
