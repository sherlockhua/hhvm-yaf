/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-03-12 17:54:07
*   description  ：
*
=============================================*/
#include "yaf_session.h"
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/ext/extension.h"
#include "hphp/runtime/ext/session/ext_session.h"
#include "hphp/runtime/base/php-globals-inl.h"
#include "ext_yaf.h"

namespace HPHP {

static Variant get_instance()
{
    Array func = Array::Create();
    func.append("Yaf_Session");
    func.append("get_instance");

    Array params = Array::Create();
    return vm_call_user_func(func, params);
}

static void set_instance(const Object& object)
{ 
    Array func = Array::Create();
    func.append("Yaf_Session");
    func.append("set_instance");

    Array params = Array::Create();
    params.append(object);
    vm_call_user_func(func, params);
}

static int yaf_session_start(const Object& session)
{
    auto ptr_status = session->o_realProp(YAF_SESSION_PROPERTY_NAME_STATUS, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    if (ptr_status->isBoolean() && ptr_status->toBoolean()) {
        return HHVM_YAF_SUCCESS;
    }

    bool ret = f_session_start();
    if (!ret) {
        raise_warning("f_session_start failed");
        return HHVM_YAF_FAILED;
    }

    *ptr_status = Variant(true);
    return HHVM_YAF_SUCCESS;
}

static Variant yaf_session_instance()
{
    Array args = Array::Create();
    Object instance = createObject("Yaf_Session", args);
    if (!instance->o_instanceof("Yaf_Session")) {
        return init_null_variant;
    }

    int ret = yaf_session_start(instance);
    if (ret != HHVM_YAF_SUCCESS) {
        return init_null_variant;
    }

    auto ptr_member = instance->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    *ptr_member = php_global(s_SESSION);

    set_instance(instance);
    return instance;
}

static void HHVM_METHOD(Yaf_Session, __construct)
{}

static void HHVM_METHOD(Yaf_Session, __sleep)
{}

static void HHVM_METHOD(Yaf_Session, __clone)
{}

static void HHVM_METHOD(Yaf_Session, __wakeup)
{}

static Variant HHVM_STATIC_METHOD(Yaf_Loader, getInstance)
{
    Variant instance = get_instance();    
    if (instance.isObject() && instance.toObject()->o_instanceof("Yaf_Session")){
        return instance;
    }


}

void YafExtension::_initYafSessionClass()
{
    HHVM_ME(Yaf_Session, __construct);
    HHVM_ME(Yaf_Session, __sleep);
    HHVM_ME(Yaf_Session, __wakeup);
    HHVM_ME(Yaf_Session, __clone);

    HHVM_STATIC_ME(Yaf_Loader, getInstance);

}


}
