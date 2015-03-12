/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-02 12:04:25
*   description  ：
*
=============================================*/
#include "yaf_router_interface.h"
#include "yaf_router_regex.h"
#include "yaf_application.h"
#include "yaf_request.h"
#include "yaf_router.h"

namespace HPHP {


static Variant yaf_route_regex_instance(
        const Object* object, const Variant& route, const Variant& def,
        const Variant& map, const Variant& verify
        )
{
    Object o;
    if (object == NULL) {
        o = *object;
    } else {
        Array params = Array::Create();
        o = createObject(String("Yaf_Route_Regex"), params);
    } 

    auto ptr_match = o->o_realProp(YAF_ROUTE_PROPETY_NAME_MATCH, 
                        ObjectData::RealPropUnchecked, "Yaf_Route_Regex");
    *ptr_match = route;

    auto ptr_def = o->o_realProp(YAF_ROUTE_PROPETY_NAME_ROUTE, 
                        ObjectData::RealPropUnchecked, "Yaf_Route_Regex");
    *ptr_def = route;

    auto ptr_map = o->o_realProp(YAF_ROUTE_PROPETY_NAME_MAP, 
                        ObjectData::RealPropUnchecked, "Yaf_Route_Regex");
    *ptr_map = map;

    auto ptr_verify = o->o_realProp(YAF_ROUTE_PROPETY_NAME_VERIFY, 
                        ObjectData::RealPropUnchecked, "Yaf_Route_Regex");

    *ptr_verify = verify;
    return o;
}


static Variant HHVM_METHOD(Yaf_Route_Regex, route, const Variant& request)
{
    if (!request.isObject()) {
        yaf_trigger_error(YAF_ERR_ROUTE_FAILED, 
                "trigger error failed, request is not object");
        return false;
    }

    return true;
}

static void HHVM_METHOD(Yaf_Route_Regex, __construct, 
        const Variant& match, const Array& route, 
        const Array& map, const Array& verify)
{
    if (!match.isString() || match.toString().length() == 0) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, "Expects a valid string"\
                " as the first parameter, in Yaf_Route_Regex");
        return;
    }

    (void)yaf_route_regex_instance(&this_, match, route, map, verify);
}

void YafExtension::_initYafRegexRouterClass()
{
    HHVM_ME(Yaf_Route_Regex, __construct);
    HHVM_ME(Yaf_Route_Regex, route);
}
 
}
