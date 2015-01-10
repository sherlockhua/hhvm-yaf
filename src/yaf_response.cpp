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
#include "yaf_response.h"
#include "ext_yaf.h"
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/base/php-globals.h"


namespace HPHP{

static void HHVM_METHOD(Yaf_Response_Abstract, __clone)
{
}

static void HHVM_METHOD(Yaf_Response_Abstract, __construct)
{
    
}

static void HHVM_METHOD(Yaf_Response_Abstract, __destruct)
{
    
}

static Variant HHVM_METHOD(Yaf_Response_Abstract, __toString)
{
    auto ptr_body = request->o_realProp(YAF_RESPONSE_PROPERTY_NAME_BODY, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    String str();
    php_implode();
    return str;`
}

void YafExtension::_initYafRequestClass()
{
    HHVM_ME(Yaf_Response_Abstract, __clone);
    HHVM_ME(Yaf_Response_Abstract, __construct);
    HHVM_ME(Yaf_Response_Abstract, __destruct);
    HHVM_ME(Yaf_Response_Abstract, __toString);

    _initYafResponseHttpClass();
    _initYafResponseClientClass();
}

}
