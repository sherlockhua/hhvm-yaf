/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-03-11 15:01:35
*   description  ：
*
=============================================*/
#include "yaf_registry.h"
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/ext/extension.h"
#include "ext_yaf.h"

namespace HPHP
{

static Variant get_instance()
{
    Array func = Array::Create();
    func.append("Yaf_Registry");
    func.append("get_instance");

    Array params = Array::Create();
    return vm_call_user_func(func, params);
}

static void set_instance(const Object& object)
{ 
    Array func = Array::Create();
    func.append("Yaf_Registry");
    func.append("set_instance");

    Array params = Array::Create();
    params.append(object);
    vm_call_user_func(func, params);
}

static Variant yaf_registry_instance()
{
    Variant instance = get_instance();
    if (instance.isNull()) {
        Array params = Array::Create();
        Object o = createObject(String("Yaf_Registry"), params);

        auto ptr_entry = o->o_realProp(YAF_REGISTRY_PROPERTY_NAME_ENTRYS, 
            ObjectData::RealPropUnchecked, "Yaf_Registry");
        *ptr_entry = Array::Create();

        set_instance(o);
        instance = o;
    }

    return instance;
}

static void HHVM_METHOD(Yaf_Registry, __construct) 
{}

static void HHVM_METHOD(Yaf_Registry, __clone)
{}


static Variant HHVM_STATIC_METHOD(Yaf_Registry, get, 
        const String& name)
{
    Variant var_instance = yaf_registry_instance();
    if (!var_instance.isObject()) {
        return false;
    }

    Object o = var_instance.toObject();
    auto ptr_entry = o->o_realProp(YAF_REGISTRY_PROPERTY_NAME_ENTRYS, 
                        ObjectData::RealPropUnchecked, "Yaf_Registry");

    if (!ptr_entry->isArray()) {
        return false;
    }

    Array& entry = ptr_entry->toArrRef();
    if (entry.exists(name)) {
        return entry[name];
    }

    return init_null_variant;
}

static Variant HHVM_STATIC_METHOD(Yaf_Registry, set, 
        const String& name, const Variant& value)
{
    Variant var_instance = yaf_registry_instance();
    if (!var_instance.isObject()) {
        return false;
    }

    Object o = var_instance.toObject();
    auto ptr_entry = o->o_realProp(YAF_REGISTRY_PROPERTY_NAME_ENTRYS, 
                        ObjectData::RealPropUnchecked, "Yaf_Registry");

    if (!ptr_entry->isArray()) {
        return false;
    }

    Array& entry = ptr_entry->toArrRef();
    entry.set(name, value);

    return true;
}

static Variant HHVM_STATIC_METHOD(Yaf_Registry, has, 
        const String& name)
{
    Variant var_instance = yaf_registry_instance();
    if (!var_instance.isObject()) {
        return false;
    }

    Object o = var_instance.toObject();
    auto ptr_entry = o->o_realProp(YAF_REGISTRY_PROPERTY_NAME_ENTRYS, 
                        ObjectData::RealPropUnchecked, "Yaf_Registry");

    if (!ptr_entry->isArray()) {
        return false;
    }

    Array& entry = ptr_entry->toArrRef();
    return entry.exists(name);
}

static Variant HHVM_STATIC_METHOD(Yaf_Registry, del, 
        const String& name)
{
    Variant var_instance = yaf_registry_instance();
    if (!var_instance.isObject()) {
        return false;
    }

    Object o = var_instance.toObject();
    auto ptr_entry = o->o_realProp(YAF_REGISTRY_PROPERTY_NAME_ENTRYS, 
                        ObjectData::RealPropUnchecked, "Yaf_Registry");

    if (!ptr_entry->isArray()) {
        return false;
    }

    Array& entry = ptr_entry->toArrRef();
    entry.remove(name);

    return true;
}

void YafExtension::_initYafRegistryClass()
{
    HHVM_ME(Yaf_Registry, __construct);
    HHVM_ME(Yaf_Registry, __clone);
    HHVM_STATIC_ME(Yaf_Registry, get);
    HHVM_STATIC_ME(Yaf_Registry, set);
    HHVM_STATIC_ME(Yaf_Registry, has);
    HHVM_STATIC_ME(Yaf_Registry, del);
}

}
