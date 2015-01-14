/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-12 11:55:50
*   description  ：
*
=============================================*/
#include "yaf_view.h"
#include "ext_yaf.h"


namespace HPHP {


static int yaf_view_simple_valid_var_name(const char *var_name, int len) /* {{{ */
{
    int i, ch;

    if (!var_name)
        return 0;

    /* These are allowed as first char: [a-zA-Z_\x7f-\xff] */
    ch = (int)((unsigned char *)var_name)[0];
    if (var_name[0] != '_' &&
            (ch < 65  /* A    */ || /* Z    */ ch > 90)  &&
            (ch < 97  /* a    */ || /* z    */ ch > 122) &&
            (ch < 127 /* 0x7f */ || /* 0xff */ ch > 255)
       ) {
        return 0;
    }

    /* And these as the rest: [a-zA-Z0-9_\x7f-\xff] */
    if (len > 1) {
        for (i = 1; i < len; i++) {
            ch = (int)((unsigned char *)var_name)[i];
            if (var_name[i] != '_' &&
                    (ch < 48  /* 0    */ || /* 9    */ ch > 57)  &&
                    (ch < 65  /* A    */ || /* Z    */ ch > 90)  &&
                    (ch < 97  /* a    */ || /* z    */ ch > 122) &&
                    (ch < 127 /* 0x7f */ || /* 0xff */ ch > 255)
               ) {
                return 0;
            }
        }
    }
    return 1;
}

static int yaf_view_simple_extract_array(const Variant& vars)
{
    int count = 0;
    if (!vars.isArray()) {
        return count;
    }

    const Array& var_array = vars.toCArrRef();
    for (ArrayIter iter(var_array); iter; ++iter) {
        std::string name = iter.first().toString().toCppString();

          /* GLOBALS protection */
        if (name.length() == sizeof("GLOBALS") && !strcmp(name.c_str(), "GLOBALS")) {
            continue;
        }   
                    
        if (name.length() == sizeof("this")  && !strcmp(name.c_str(), "this")) {
            continue;
        }   

        if (!yaf_view_simple_valid_var_name(name.c_str(), name.length())) {
                continue;
        }

        g_context->setVar(iter.first().toString().get(), iter.secondRef().asTypedValue());
        ++count;
    }

    return count;
}

static int yaf_view_simple_extract(const Variant& tpl_vars, const Variant& vars)
{
    return yaf_view_simple_extract_array(tpl_vars) + yaf_view_simple_extract_array(vars);
}

static Variant yaf_view_simple_render(ObjectData* object, 
        const Variant& tpl, const Variant& vars)
{
    if (!tpl.isString()) {
        return Variant(false);
    }

    auto ptr_tplvars = object->o_realProp(YAF_VIEW_PROPERTY_NAME_TPLVARS, 
            ObjectData::RealPropUnchecked, "Yaf_View_Simple");

    yaf_view_simple_extract(*ptr_tplvars, vars);
    return Variant(true);
}

static void yaf_view_simple_instance(ObjectData* object, const Variant& tpl_dir,
        const Variant& options)
{
    auto ptr_tplvars = object->o_realProp(YAF_VIEW_PROPERTY_NAME_TPLVARS, 
            ObjectData::RealPropUnchecked, "Yaf_View_Simple");
    *ptr_tplvars = Array::Create();

    auto ptr_tpldir = object->o_realProp(YAF_VIEW_PROPERTY_NAME_TPLDIR, 
            ObjectData::RealPropUnchecked, "Yaf_View_Simple");

    if (tpl_dir.isString()) {
        const String& str_tpl_dir = tpl_dir.toCStrRef();
        if (str_tpl_dir.length()) {
            if (IS_ABSOLUTE_PATH(str_tpl_dir)) {
                *ptr_tpldir = str_tpl_dir;
            } else {
                raise_error("template_dir expect a absolute path:%s", str_tpl_dir.toCppString().c_str());
            }
        }
    }

    if (options.isArray()) {
        auto ptr_options = object->o_realProp(YAF_VIEW_PROPERTY_NAME_OPTS, 
                ObjectData::RealPropUnchecked, "Yaf_View_Simple");
        *ptr_options = options;
    } 

}

static void HHVM_METHOD(Yaf_View_Simple, __construct, const Variant& tpl_dir,
        const Variant& options)
{
    yaf_view_simple_instance(this_, tpl_dir, options);
}

static bool HHVM_METHOD(Yaf_View_Simple, __isset, const String& name)
{
    auto ptr_tplvars = this_->o_realProp(YAF_VIEW_PROPERTY_NAME_TPLVARS, 
            ObjectData::RealPropUnchecked, "Yaf_View_Simple");
    if (!ptr_tplvars->isArray()) {
        return false;
    }

    return ptr_tplvars->toArrRef().exists(name);
}

static Variant HHVM_METHOD(Yaf_View_Simple, get, const Variant& name)
{
    auto ptr_tplvars = this_->o_realProp(YAF_VIEW_PROPERTY_NAME_TPLVARS, 
            ObjectData::RealPropUnchecked, "Yaf_View_Simple");
    if (!ptr_tplvars->isArray()) {
        return init_null_variant;
    }

    if (name.isString() && name.toCStrRef().length()) {
        const String& str_name = name.toCStrRef();
        if (ptr_tplvars->toArrRef().exists(str_name)) {
            return ptr_tplvars->toArrRef()[str_name];
        } 
    } else {
        return *ptr_tplvars; 
    }

    return init_null_variant;
}

static Variant HHVM_METHOD(Yaf_View_Simple, assign, const Variant& name, const Variant& value)
{
    auto ptr_tplvars = this_->o_realProp(YAF_VIEW_PROPERTY_NAME_TPLVARS, 
            ObjectData::RealPropUnchecked, "Yaf_View_Simple");

    if (name.isString()) {
        ptr_tplvars->toArrRef().set(name, value);
        return true;
    } else if(name.isArray()){
        ptr_tplvars->toArrRef().merge(name.toArray());
        return true;
    }

    return false;
}

static Variant HHVM_METHOD(Yaf_View_Simple, render, const Variant& tpl, const Variant& vars)
{
    return yaf_view_simple_render(this_, tpl, vars);
}

void YafExtension::_initYafViewSimpleClass()
{
    HHVM_ME(Yaf_View_Simple, __construct);
    HHVM_ME(Yaf_View_Simple, __isset);
    HHVM_ME(Yaf_View_Simple, get);
    HHVM_ME(Yaf_View_Simple, assign);
    HHVM_ME(Yaf_View_Simple, render);

}


}
