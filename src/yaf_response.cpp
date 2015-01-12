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

static int yaf_response_send(ObjectData* object)
{

    auto ptr_body = object->o_realProp(YAF_RESPONSE_PROPERTY_NAME_BODY, 
            ObjectData::RealPropUnchecked, "Yaf_Response_Abstract");

    if (!ptr_body->isArray()) {
        return 1;
    }

    Array& body_array = ptr_body->toArrRef();

    ArrayIter iter = body_array.begin();
    while (!iter.end()) {
        Variant value = iter.second();
        String str_value = value.toString();
        g_context->write(str_value);
        iter.next();
    }

    return 0;
}

int yaf_response_set_redirect(ObjectData* response, const String& location)
{
    Transport *transport = g_context->getTransport();
    if (transport) {
        transport->replaceHeader("Location", location.toCppString().c_str());
    }

    return 0;
}

static int yaf_response_alter_body(ObjectData* object, 
        const Variant& name, const String& body, int flag)
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

static int yaf_response_clear_body(ObjectData* object, const Variant& name)
{

    auto ptr_body = object->o_realProp(YAF_RESPONSE_PROPERTY_NAME_BODY, 
            ObjectData::RealPropUnchecked, "Yaf_Response_Abstract");

    if (!ptr_body->isArray()) {
        return 1;
    }

    if (name.isString()&&name.toString().length()) {
        ptr_body->toArrRef().remove(name.toString(), true);
    } else {
        ptr_body->toArrRef().clear();
    }

    return 0;
}

static Variant yaf_response_get_body(ObjectData* object, const Variant& name)
{

    auto ptr_body = object->o_realProp(YAF_RESPONSE_PROPERTY_NAME_BODY, 
            ObjectData::RealPropUnchecked, "Yaf_Response_Abstract");

    if (!ptr_body->isArray()) {
        return init_null_variant;
    }

    if (!name.isString() || name.toString().length() == 0) {
        return *ptr_body;
    }

    if (ptr_body->toArray().exists(name.toString())) {
        return ptr_body->toArrRef()[name.toString()];
    }

    return init_null_variant;
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

//TODO check name for NULL
static Variant HHVM_METHOD(Yaf_Response_Abstract, getBody, const Variant& name)
{
    String key;
    if (name.isNull()) {
        key = String(YAF_RESPONSE_PROPERTY_NAME_DEFAULTBODY);
    } else {
        key = name.toString();
    }

    Variant value = yaf_response_get_body(this_, key);
    return value;
}

static Variant HHVM_METHOD(Yaf_Response_Abstract, setBody, 
        const String& body, const Variant& name)
{
    int ret = yaf_response_alter_body(this_, name, body, YAF_RESPONSE_REPLACE);
    if (ret != 0) {
        return false;
    }

    return this_;
}

static Variant HHVM_METHOD(Yaf_Response_Abstract, appendBody, 
        const String& body, const Variant& name)
{
    int ret = yaf_response_alter_body(this_, name, body, YAF_RESPONSE_APPEND);
    if (ret != 0) {
        return false;
    }

    return this_;
}

static Variant HHVM_METHOD(Yaf_Response_Abstract, prependBody, 
        const String& body, const Variant& name)
{
    int ret = yaf_response_alter_body(this_, name, body, YAF_RESPONSE_PREPEND);
    if (ret != 0) {
        return false;
    }

    return this_;
}

static Variant HHVM_METHOD(Yaf_Response_Abstract, clearBody, const Variant& name)
{
    int ret = yaf_response_clear_body(this_, name);
    if (ret != 0) {
        return false;
    }

    return this_;
}

static Variant HHVM_METHOD(Yaf_Response_Abstract, setHeader)
{
    return false;
}

static Variant HHVM_METHOD(Yaf_Response_Abstract, setAllHeaders)
{
    return false;
}

static Variant HHVM_METHOD(Yaf_Response_Abstract, getHeader)
{
    return false;
}

static Variant HHVM_METHOD(Yaf_Response_Abstract, clearHeaders)
{
    return false;
}

static bool HHVM_METHOD(Yaf_Response_Abstract, setRedirect, const String& url)
{
    if (yaf_response_set_redirect(this_, url) == 0) {
        return true;
    }

    return false;
}

static Variant HHVM_METHOD(Yaf_Response_Abstract, response)
{
    if (yaf_response_send(this_) == 0) {
        return true;
    }

    return false;
}

void YafExtension::_initYafResponseClass()
{
    HHVM_ME(Yaf_Response_Abstract, __clone);
    HHVM_ME(Yaf_Response_Abstract, __construct);
    HHVM_ME(Yaf_Response_Abstract, __destruct);
    HHVM_ME(Yaf_Response_Abstract, __toString);
    HHVM_ME(Yaf_Response_Abstract, getBody);
    HHVM_ME(Yaf_Response_Abstract, setBody);
    HHVM_ME(Yaf_Response_Abstract, appendBody);
    HHVM_ME(Yaf_Response_Abstract, prependBody);
    HHVM_ME(Yaf_Response_Abstract, clearBody);

    HHVM_ME(Yaf_Response_Abstract, setHeader);
    HHVM_ME(Yaf_Response_Abstract, setAllHeaders);
    HHVM_ME(Yaf_Response_Abstract, getHeader);
    HHVM_ME(Yaf_Response_Abstract, clearHeaders);

    HHVM_ME(Yaf_Response_Abstract, setRedirect);
    HHVM_ME(Yaf_Response_Abstract, response);

    _initYafResponseHttpClass();
    _initYafResponseClientClass();
}

}
