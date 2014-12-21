/*===========================================
*   Copyright (C) 2014 All rights reserved.
*   
*   company      : xiaomi
*   author       : root
*   email        : root@xiaomi.com
*   date         ：2014-12-18 07:29:18
*   description  ：
*
=============================================*/
#include "yaf_request.h"
#include "ext_yaf.h"
#include "hphp/runtime/base/base-includes.h"


namespace HPHP{

int yaf_request_set_dispatched(Object request, int flag)
{
    auto var_flag = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_STATE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
    *var_flag = Variant(flag);

    return 0;
}


void YafExtension::_initYafRequestClass()
{

    _initYafRequestHttpClass();
    _initYafRequestSimpleClass();
}

}
