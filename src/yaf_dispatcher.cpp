/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-12 11:03:59
*   description  ：
*
=============================================*/
#include "yaf_dispatcher.h"
#include "ext_yaf.h"

namespace HPHP {


Variant yaf_dispatcher_dispatch(Object* object)
{
    return init_null_variant;
}

Variant yaf_dispatcher_instance(Object* object)
{
    return init_null_variant;
}

Variant yaf_dispatcher_set_request(Object* object, Object* request)
{
    return init_null_variant;
}

static Variant yaf_dispatcher_init_view(const Object* object, 
        const Variant& tpl_dir, const Variant& options)
{
    return init_null_variant;
}


static void HHVM_METHOD(Yaf_Dispatcher, __clone)
{
}

static void HHVM_METHOD(Yaf_Dispatcher, __construct)
{
}

static void HHVM_METHOD(Yaf_Dispatcher, __sleep)
{
}

static void HHVM_METHOD(Yaf_Dispatcher, __wakeup)
{
}

static Variant HHVM_METHOD(Yaf_Dispatcher, enableView)
{
    auto ptr_render = this_->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_RENDER, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    *ptr_render = Variant(true);
    return this_;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, disableView)
{
    auto ptr_render = this_->o_realProp(YAF_DISPATCHER_PROPERTY_NAME_RENDER, 
            ObjectData::RealPropUnchecked, "Yaf_Dispatcher");

    *ptr_render = Variant(false);
    return this_;
}

static Variant HHVM_METHOD(Yaf_Dispatcher, initView, const Variant& tpl_dir, 
        const Variant& options)
{
    Variant view = yaf_dispatcher_init_view(&this_, tpl_dir, options);
    if (!view.isNull()) {
        return view;
    }

    return false;
}

void YafExtension::_initYafDispatcherClass()
{
    HHVM_ME(Yaf_Dispatcher, __clone);
    HHVM_ME(Yaf_Dispatcher, __construct);
    HHVM_ME(Yaf_Dispatcher, __sleep);
    HHVM_ME(Yaf_Dispatcher, __wakeup);

    HHVM_ME(Yaf_Dispatcher, enableView);
    HHVM_ME(Yaf_Dispatcher, disableView);
    HHVM_ME(Yaf_Dispatcher, initView);

}


}
