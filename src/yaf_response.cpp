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

static int yaf_response_alter_body(ObjectData* object, 
        const Variant name, const String& body, int flag)
{
    if (!body.length()) {
        return 1;
    }

    String body_name(YAF_RESPONSE_PROPERTY_NAME_DEFAULTBODY);
    if (name.isString()&&name.toString().length()) {
        body_name = name.toString();
    }

    auto ptr_body = object->o_realProp(YAF_RESPONSE_PROPERTY_NAME_BODY, 
            ObjectData::RealPropUnchecked, "Yaf_Response_Abstract");

    if (!ptr_body->isArray()) {
        *ptr_body = Array::Create();
    }

    Array& ref_body = ptr_body->toArrRef();
    if (!ref_body.exists(body_name)) {
        ref_body.set(body_name, String(""));
    }

    String& value = ref_body.lvalAt(body_name).toStrRef();
    switch(flag) {
        case  YAF_RESPONSE_PREPEND:
            value = body + value;
            break;
        case YAF_RESPONSE_APPEND:
            value = value + body;
            break;
        case YAF_RESPONSE_REPLACE:
        default:
            value = body;
            break;
    }

    return 0;
}

static void HHVM_METHOD(Yaf_Response_Abstract, __clone)
{
}

static void HHVM_METHOD(Yaf_Response_Abstract, __construct)
{
    
}

static void HHVM_METHOD(Yaf_Response_Abstract, __destruct)
{
    
}

static String HHVM_METHOD(Yaf_Response_Abstract, __toString)
{
    auto ptr_body = this_->o_realProp(YAF_RESPONSE_PROPERTY_NAME_BODY, 
            ObjectData::RealPropUnchecked, "Yaf_Response_Abstract");

    if (!ptr_body->isArray()) {
        raise_warning("Expect _body become array");
        return false;
    }

    Array items = ptr_body->toArray();
    String delim("");
    return StringUtil::Implode(items, delim);
}

static Variant HHVM_METHOD(Yaf_Response_Abstract, getBody)
{
   return String("abcddd"); 
}

static bool HHVM_METHOD(Yaf_Response_Abstract, setBody, 
        const String& body, const Variant& name)
{
    int ret = yaf_response_alter_body(this_, name, body, YAF_RESPONSE_APPEND);
    if (ret != 0) {
        return false;
    }

    return true;
}

void YafExtension::_initYafResponseClass()
{
    HHVM_ME(Yaf_Response_Abstract, __clone);
    HHVM_ME(Yaf_Response_Abstract, __construct);
    HHVM_ME(Yaf_Response_Abstract, __destruct);
    HHVM_ME(Yaf_Response_Abstract, __toString);
    HHVM_ME(Yaf_Response_Abstract, getBody);
    HHVM_ME(Yaf_Response_Abstract, setBody);

    _initYafResponseHttpClass();
    _initYafResponseClientClass();
}

}
