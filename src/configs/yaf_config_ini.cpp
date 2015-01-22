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
static Variant yaf_config_ini_instance(ObjectData* object, 
#else
static Variant yaf_config_ini_instance(Object* object, 
#endif
        const String& filename, const Variant& section)
{
    Object o;
    if (object == NULL) {
        Array arr = Array::Create();
        arr.append(filename);
        arr.append(section);

        o = createObject("Yaf_Config_Ini", arr) ;
    } else {
        o = *object;
    }

    if (config.isArray()) {
        auto ptr_config = o->o_realProp(YAF_CONFIG_PROPERT_NAME, 
                ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
        *ptr_config = config;
        if (readonly.isBoolean()) {
            auto ptr_readonly = o->o_realProp(YAF_CONFIG_PROPERT_NAME_READONLY, 
                    ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
            *ptr_readonly = readonly.toBoolean();
        } 
    } else {
        raise_error("config expect is array");
        return false;
    }

    return o;
}

static void HHVM_METHOD(Yaf_Config_Ini, __construct, 
        const String& filename, const Variant& section)
{
    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");

    if (!ptr_config->isArray()) {
        *ptr_config = Array::Create();
    }

    (void)yaf_config_ini_instance(&this_, filename, section);
}

void YafExtension::_initYafConfigIniClass()
{
    HHVM_ME(Yaf_Config_Ini, __construct);
}

}
