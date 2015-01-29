/*===========================================
*   Copyright (C) 2014 All rights reserved.
*   
*   company      : xiaomi
*   author       : root
*   email        : root@xiaomi.com
*   date         ：2014-12-13 09:48:47
*   description  ：
*
=============================================*/
//#include "ext_example.h"
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/ext/extension.h"
#include "ext_yaf.h"
#include "yaf_config.h"

namespace HPHP { 

#ifdef HHVM_VERSION_3_2_NEW
static Variant yaf_config_simple_instance(ObjectData* object, 
#else
static Variant yaf_config_simple_instance(const Object* object, 
#endif
        const Variant& config, const Variant& readonly)
{
    Object o;
    if (object == NULL) {
        Array arr = Array::Create();
        arr.append(Array::Create());
        arr.append(Variant(true));

        o = createObject("Yaf_Config_Simple", arr) ;
    } else {
        o = *object;
    }

    if (config.isArray()) {
        auto ptr_config = o->o_realProp(YAF_CONFIG_PROPERT_NAME, 
                ObjectData::RealPropUnchecked, "Yaf_Config_Simple");
        *ptr_config = config;
        if (readonly.isBoolean()) {
            auto ptr_readonly = o->o_realProp(YAF_CONFIG_PROPERT_NAME_READONLY, 
                    ObjectData::RealPropUnchecked, "Yaf_Config_Simple");
            *ptr_readonly = readonly.toBoolean();
        } 
    } else {
        raise_error("config expect is array");
        return false;
    }

    return o;
}

#ifdef HHVM_VERSION_3_2_NEW
static Variant yaf_config_simple_format(ObjectData* object, 
#else
static Variant yaf_config_simple_format(const Object* object, 
#endif
        const Variant& config)
{
    auto ptr_readonly = (*object)->o_realProp(YAF_CONFIG_PROPERT_NAME_READONLY, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");
    return yaf_config_simple_instance(NULL, config, *ptr_readonly);
}

static void HHVM_METHOD(Yaf_Config_Simple, __construct, 
        const Variant& config, const Variant& readonly)
{
    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");

    if (!ptr_config->isArray()) {
        *ptr_config = Array::Create();
    }

    (void)yaf_config_simple_instance(&this_, config, readonly);
}

static bool HHVM_METHOD(Yaf_Config_Simple, __isset, const String& name)
{
    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");

    if (!ptr_config->isArray()) {
        *ptr_config = Array::Create();
        return false;
    } 

    Array& arr = ptr_config->toArrRef();
    return arr.exists(name);
}

static Variant HHVM_METHOD(Yaf_Config_Simple, get, const Variant& name)
{
    if (!name.isString() || (name.toString().length() == 0)) {
        return this_;
    }

    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");

    if (!ptr_config->isArray()) {
        *ptr_config = Array::Create();
        return false;
    } 

    Array& arr = ptr_config->toArrRef();
    if (!arr.exists(name.toString())) {
        return false;
    }

    Variant value = arr[name.toString()];
    if (value.isArray()) {
        Variant instance = yaf_config_simple_format(&this_, value);
        return instance;
    }

    return value;
}

static Variant HHVM_METHOD(Yaf_Config_Simple, set, 
        const String& name, const Variant& value)
{
    auto ptr_readonly = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_READONLY, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");

    if (ptr_readonly->toBoolean()) {
        return false;
    } 

    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");
    Array& arr = ptr_config->toArrRef();
    arr.set(name, value);

    return value;
}

static Variant HHVM_METHOD(Yaf_Config_Simple, count) 
{
    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");
    if (!ptr_config->isArray()) {
        return 0;
    }

    Array& arr = ptr_config->toArrRef();
    return arr.size();
}

static Variant HHVM_METHOD(Yaf_Config_Simple, offsetUnset, const String& name) 
{
    auto ptr_readonly = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_READONLY, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");

    if (ptr_readonly->toBoolean()) {
        return false;
    } 

    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");
    if (!ptr_config->isArray()) {
        return false;
    }

    Array& arr = ptr_config->toArrRef();
    arr.remove(name);
    return true;
}

static Variant HHVM_METHOD(Yaf_Config_Simple, rewind) 
{
    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");
    if (!ptr_config->isArray()) {
        return false;
    }

    Array& arr = ptr_config->toArrRef();
    auto ptr_cursor = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_CURSOR, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");
    *ptr_cursor = Variant(NEWOBJ(yaf_config_cursor)(arr.begin()));
    return true;
}

static Variant HHVM_METHOD(Yaf_Config_Simple, current) 
{
    auto ptr_cursor = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_CURSOR, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");

    auto res = ptr_cursor->toResource();
    ArrayIter& cursor = res.getTyped<yaf_config_cursor>()->cursor();
    Variant value =  cursor.second();
    if (!value.isArray()) {
        return value;
    }

    return yaf_config_simple_format(&this_, value);
}

static Variant HHVM_METHOD(Yaf_Config_Simple, next) 
{
    auto ptr_cursor = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_CURSOR, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");
    if (ptr_cursor->isNull()) {
        return false;
    }
    
    auto res = ptr_cursor->toResource();
    ArrayIter& cursor = res.getTyped<yaf_config_cursor>()->cursor();
    cursor.next();
    return true;
}

static Variant HHVM_METHOD(Yaf_Config_Simple, valid) 
{
    auto ptr_cursor = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_CURSOR, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");
    if (ptr_cursor->isNull()) {
        return false;
    }
 
    auto res = ptr_cursor->toResource();
    ArrayIter& cursor = res.getTyped<yaf_config_cursor>()->cursor();
    return !cursor.end();
}

static Variant HHVM_METHOD(Yaf_Config_Simple, readonly) 
{
    auto ptr_readonly = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_READONLY, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");
    if (ptr_readonly->isNull()) {
        return false;
    }
 
    return ptr_readonly->toBoolean();
}

static Variant HHVM_METHOD(Yaf_Config_Simple, toArray) 
{
    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");
    if (!ptr_config->isArray()) {
        return false;
    }
 
    return ptr_config->toArray();
}

static bool HHVM_METHOD(Yaf_Config_Simple, offsetExists, const String& name)
{
    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");

    if (!ptr_config->isArray()) {
        *ptr_config = Array::Create();
        return false;
    } 

    Array& arr = ptr_config->toArrRef();
    return arr.exists(name);
}

static Variant HHVM_METHOD(Yaf_Config_Simple, offsetSet, 
        const String& name, const Variant& value)
{
    auto ptr_readonly = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_READONLY, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");

    if (ptr_readonly->toBoolean()) {
        return false;
    } 

    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");
    Array& arr = ptr_config->toArrRef();
    arr.set(name, value);

    return value;
}

static Variant HHVM_METHOD(Yaf_Config_Simple, __set, 
        const String& name, const Variant& value)
{
    auto ptr_readonly = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_READONLY, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");

    if (ptr_readonly->toBoolean()) {
        return false;
    } 

    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");
    Array& arr = ptr_config->toArrRef();
    arr.set(name, value);

    return value;
}

static Variant HHVM_METHOD(Yaf_Config_Simple, __get, const Variant& name)
{
    if (!name.isString() || (name.toString().length() == 0)) {
        return this_;
    }

    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");

    if (!ptr_config->isArray()) {
        *ptr_config = Array::Create();
        return false;
    } 

    Array& arr = ptr_config->toArrRef();
    if (!arr.exists(name.toString())) {
        return false;
    }

    Variant value = arr[name.toString()];
    if (value.isArray()) {
        Variant instance = yaf_config_simple_format(&this_, value);
        return instance;
    }

    return value;
}

static Variant HHVM_METHOD(Yaf_Config_Simple, offsetGet, const Variant& name)
{
    if (!name.isString() || (name.toString().length() == 0)) {
        return this_;
    }

    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");

    if (!ptr_config->isArray()) {
        *ptr_config = Array::Create();
        return false;
    } 

    Array& arr = ptr_config->toArrRef();
    if (!arr.exists(name.toString())) {
        return false;
    }

    Variant value = arr[name.toString()];
    if (value.isArray()) {
        Variant instance = yaf_config_simple_format(&this_, value);
        return instance;
    }

    return value;
}

static Variant HHVM_METHOD(Yaf_Config_Simple, key)
{
    auto ptr_cursor = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_CURSOR, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Simple");

    if (ptr_cursor->isNull()) {
        auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
                ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
        if (!ptr_config->isArray()) {
            return false;
        }

        Array& arr = ptr_config->toArrRef();
        auto ptr_cursor = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_CURSOR, 
                ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
        *ptr_cursor = Variant(NEWOBJ(yaf_config_cursor)(arr.begin()));
    } 

    auto res = ptr_cursor->toResource();
    ArrayIter& cursor = res.getTyped<yaf_config_cursor>()->cursor();

    if (cursor.first().isString()) {
        return cursor.first().toString();
    } else if (cursor.first().isInteger()) {
        return cursor.first().toInt64();
    }

    return cursor.first();
}

void YafExtension::_initYafConfigSimpleClass()
{
    HHVM_ME(Yaf_Config_Simple, __construct);
    HHVM_ME(Yaf_Config_Simple, __isset);
    HHVM_ME(Yaf_Config_Simple, get);
    HHVM_ME(Yaf_Config_Simple, set);
    HHVM_ME(Yaf_Config_Simple, count);
    HHVM_ME(Yaf_Config_Simple, offsetUnset);

    HHVM_ME(Yaf_Config_Simple, rewind);
    HHVM_ME(Yaf_Config_Simple, current);
    HHVM_ME(Yaf_Config_Simple, next);
    HHVM_ME(Yaf_Config_Simple, valid);
    HHVM_ME(Yaf_Config_Simple, key);

    HHVM_ME(Yaf_Config_Simple, readonly);
    HHVM_ME(Yaf_Config_Simple, toArray);

    HHVM_ME(Yaf_Config_Simple, __set);
    HHVM_ME(Yaf_Config_Simple, __get);
    HHVM_ME(Yaf_Config_Simple, offsetGet);
    HHVM_ME(Yaf_Config_Simple, offsetExists);
    HHVM_ME(Yaf_Config_Simple, offsetSet);

}

}
