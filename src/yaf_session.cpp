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

void yaf_session_cursor::sweep() {
  Close();
}

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

    Array params = Array::Create();
    Variant ret = vm_call_user_func(String("session_start"), params);
    //bool ret = f_session_start();
    if (ret.isBoolean() && ret.toBoolean() == false) {
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
    if (!instance.instanceof("Yaf_Session")) {
        return init_null_variant;
    }

    int ret = yaf_session_start(instance);
    if (ret != HHVM_YAF_SUCCESS) {
        return init_null_variant;
    }

    auto ptr_member = instance->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");


    auto tv = g_context->m_globalVarEnv->lookup(s_SESSION.get());
    ptr_member->constructRefHelper(*(Variant*)(tv));

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

static Variant HHVM_STATIC_METHOD(Yaf_Session, getInstance)
{
    Variant instance = get_instance();    
    if (instance.isObject() && 
            Yaf_Common_InstanceOf(instance.toObject(), String("Yaf_Session"))){
        return instance;
    }

    return yaf_session_instance();
}

static Variant HHVM_METHOD(Yaf_Session, start)
{
    yaf_session_start(this_);
    return this_;
}

static Variant HHVM_METHOD(Yaf_Session, get, const Variant& name)
{
    auto ptr_session = this_->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    if (!name.isString() || name.toString().length() == 0) {
        return ptr_session->toArray();
    }

    Array& session = ptr_session->toArrRef();
    if (!session.exists(name.toString())) {
        return init_null_variant;
    }

    return session[name.toString()];
}

static Variant HHVM_METHOD(Yaf_Session, offsetGet, const Variant& name)
{
    auto ptr_session = this_->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    if (!name.isString() || name.toString().length() == 0) {
        return ptr_session->toArray();
    }

    Array& session = ptr_session->toArrRef();
    if (!session.exists(name.toString())) {
        return init_null_variant;
    }

    return session[name.toString()];
}

static Variant HHVM_METHOD(Yaf_Session, __get, const Variant& name)
{
    auto ptr_session = this_->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    if (!name.isString() || name.toString().length() == 0) {
        return ptr_session->toArray();
    }

    Array& session = ptr_session->toArrRef();
    if (!session.exists(name.toString())) {
        return init_null_variant;
    }

    return session[name.toString()];
}

static Variant HHVM_METHOD(Yaf_Session, has, const Variant& name)
{
    auto ptr_session = this_->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    Array& session = ptr_session->toArrRef();
    return session.exists(name.toString());
}

static Variant HHVM_METHOD(Yaf_Session, offsetExists, const Variant& name)
{
    auto ptr_session = this_->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    Array& session = ptr_session->toArrRef();
    return session.exists(name.toString());
}

static Variant HHVM_METHOD(Yaf_Session, __isset, const Variant& name)
{
    auto ptr_session = this_->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    Array& session = ptr_session->toArrRef();
    return session.exists(name.toString());
}

static Variant HHVM_METHOD(Yaf_Session, set, 
        const Variant& name, const Variant& value)
{
    auto ptr_session = this_->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    Array& session = ptr_session->toArrRef();
    session.set(name, value);
    return true;
}

static Variant HHVM_METHOD(Yaf_Session, offsetSet, 
        const Variant& name, const Variant& value)
{
    auto ptr_session = this_->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    Array& session = ptr_session->toArrRef();
    session.set(name, value);
    return true;
}

static Variant HHVM_METHOD(Yaf_Session, __set, 
        const Variant& name, const Variant& value)
{
    auto ptr_session = this_->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    Array& session = ptr_session->toArrRef();
    session.set(name, value);
    return true;
}

static Variant HHVM_METHOD(Yaf_Session, del, 
        const Variant& name)
{
    auto ptr_session = this_->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    Array& session = ptr_session->toArrRef();
    session.remove(name);
    return true;
}

static Variant HHVM_METHOD(Yaf_Session, offsetUnset, 
        const Variant& name)
{
    auto ptr_session = this_->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    Array& session = ptr_session->toArrRef();
    session.remove(name);
    return true;
}

static Variant HHVM_METHOD(Yaf_Session, __unset, 
        const Variant& name)
{
    auto ptr_session = this_->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    Array& session = ptr_session->toArrRef();
    session.remove(name);
    return true;
}

static Variant HHVM_METHOD(Yaf_Session, count) 
{
    auto ptr_session = this_->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    Array& session = ptr_session->toArrRef();
    return session.size();
}

static Variant HHVM_METHOD(Yaf_Session, rewind)
{
    auto ptr_session = this_->o_realProp(YAF_SESSION_PROPERTY_NAME_SESSION, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    Array& session = ptr_session->toArrRef();

    auto ptr_cursor = this_->o_realProp(YAF_SESSION_PROPERT_NAME_CURSOR, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    *ptr_cursor = Variant(NEWOBJ(yaf_session_cursor)(session.begin()));
    //*ptr_cursor = Variant(newres<yaf_session_cursor>(session.begin()));
    return true;
}

static Variant HHVM_METHOD(Yaf_Session, next)
{
    auto ptr_cursor = this_->o_realProp(YAF_SESSION_PROPERT_NAME_CURSOR, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    if (!ptr_cursor->isResource()) {
        return false;
    }

    auto res = ptr_cursor->toResource();
    ArrayIter& cursor = res.getTyped<yaf_session_cursor>()->cursor();
    cursor.next();
    return true;
}

static Variant HHVM_METHOD(Yaf_Session, current)
{
    auto ptr_cursor = this_->o_realProp(YAF_SESSION_PROPERT_NAME_CURSOR, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    if (!ptr_cursor->isResource()) {
        return false;
    }

    auto res = ptr_cursor->toResource();
    ArrayIter& cursor = res.getTyped<yaf_session_cursor>()->cursor();
    return cursor.second();
}

static Variant HHVM_METHOD(Yaf_Session, key)
{
    auto ptr_cursor = this_->o_realProp(YAF_SESSION_PROPERT_NAME_CURSOR, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    if (!ptr_cursor->isResource()) {
        return false;
    }

    auto res = ptr_cursor->toResource();
    ArrayIter& cursor = res.getTyped<yaf_session_cursor>()->cursor();
    return cursor.first();
}

static Variant HHVM_METHOD(Yaf_Session, valid)
{
    auto ptr_cursor = this_->o_realProp(YAF_SESSION_PROPERT_NAME_CURSOR, 
            ObjectData::RealPropUnchecked, "Yaf_Session");

    if (!ptr_cursor->isResource()) {
        return false;
    }

    auto res = ptr_cursor->toResource();
    ArrayIter& cursor = res.getTyped<yaf_session_cursor>()->cursor();
    return cursor.end() == false;
}



void YafExtension::_initYafSessionClass()
{
    HHVM_ME(Yaf_Session, __construct);
    HHVM_ME(Yaf_Session, __sleep);
    HHVM_ME(Yaf_Session, __wakeup);
    HHVM_ME(Yaf_Session, __clone);

    HHVM_STATIC_ME(Yaf_Session, getInstance);

    HHVM_ME(Yaf_Session, start);
    HHVM_ME(Yaf_Session, get);
    HHVM_ME(Yaf_Session, __get);
    HHVM_ME(Yaf_Session, offsetGet);

    HHVM_ME(Yaf_Session, has);
    HHVM_ME(Yaf_Session, __isset);
    HHVM_ME(Yaf_Session, offsetExists);

    HHVM_ME(Yaf_Session, set);
    HHVM_ME(Yaf_Session, __set);
    HHVM_ME(Yaf_Session, offsetSet);

    HHVM_ME(Yaf_Session, del);
    HHVM_ME(Yaf_Session, __unset);
    HHVM_ME(Yaf_Session, offsetUnset);

    HHVM_ME(Yaf_Session, count);
    HHVM_ME(Yaf_Session, next);
    HHVM_ME(Yaf_Session, current);
    HHVM_ME(Yaf_Session, key);
    HHVM_ME(Yaf_Session, valid);

    HHVM_ME(Yaf_Session, rewind);

}


}
