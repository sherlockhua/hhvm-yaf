/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-12 10:29:15
*   description  ：
*
=============================================*/
#include "yaf_application.h"
#include "yaf_config.h"
#include "ext_yaf.h"


namespace HPHP{

#ifdef HHVM_VERSION_3_2_NEW
static Variant yaf_config_instance(ObjectData* object, const Variant& config, const String& section)
#else
static Variant yaf_config_instance(Object* object, const Variant& config, const String& section)
#endif
{
    if (config.isArray()) {

    }
    return true;
}

static void HHVM_METHOD(Yaf_Application, __clone)
{
}

static void HHVM_METHOD(Yaf_Application, __construct, const Variant& config, 
        const Variant& section)
{
    auto ptr_app = this_->o_realProp(YAF_APPLICATION_PROPERTY_NAME_APP, 
            ObjectData::RealPropUnchecked, "Yaf_Application");

    if (!ptr_app->isNull()) {
        yaf_trigger_error(YAF_ERR_STARTUP_FAILED, 
                "Only one application can be initialized");
        return;
    }

    String str_section;
    if (!section.isString() ||
            (section.isString() && section.toString().length() == 0)) {
        str_section = String(g_yaf_local_data.get()->environ.c_str());
    } else {
        str_section = section.toString();
    }

    Variant var_config = yaf_config_instance(NULL, config, str_section);
}

void YafExtension::_initYafApplicationClass()
{
    HHVM_ME(Yaf_Application, __clone);
    HHVM_ME(Yaf_Application, __construct);
}

}
