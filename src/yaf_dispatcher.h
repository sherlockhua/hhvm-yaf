/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-12 11:03:39
*   description  ：
*
=============================================*/
#ifndef _YAF_DISPATCHER_H_
#define _YAF_DISPATCHER_H_

#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/ext/extension.h"

#define YAF_DISPATCHER_PROPERTY_NAME_REQUEST    "_request"
#define YAF_DISPATCHER_PROPERTY_NAME_VIEW       "_view"
#define YAF_DISPATCHER_PROPERTY_NAME_ROUTER     "_router"
#define YAF_DISPATCHER_PROPERTY_NAME_INSTANCE   "_instance"
#define YAF_DISPATCHER_PROPERTY_NAME_RENDER     "_auto_render"
#define YAF_DISPATCHER_PROPERTY_NAME_RETURN     "_return_response"
#define YAF_DISPATCHER_PROPERTY_NAME_FLUSH      "_instantly_flush"
#define YAF_DISPATCHER_PROPERTY_NAME_ARGS       "_invoke_args"

#define YAF_DISPATCHER_PROPERTY_NAME_MODULE     "_default_module"
#define YAF_DISPATCHER_PROPERTY_NAME_CONTROLLER "_default_controller"
#define YAF_DISPATCHER_PROPERTY_NAME_ACTION     "_default_action"

#define YAF_ERROR_CONTROLLER                    "Error"
#define YAF_ERROR_ACTION                        "error"

#define YAF_DISPATCHER_PROPERTY_NAME_PLUGINS        "_plugins"

#define YAF_PLUGIN_HOOK_ROUTESTARTUP                "routerstartup"
#define YAF_PLUGIN_HOOK_ROUTESHUTDOWN           "routershutdown"
#define YAF_PLUGIN_HOOK_LOOPSTARTUP             "dispatchloopstartup"
#define YAF_PLUGIN_HOOK_PREDISPATCH                 "predispatch"
#define YAF_PLUGIN_HOOK_POSTDISPATCH                "postdispatch"
#define YAF_PLUGIN_HOOK_LOOPSHUTDOWN                "dispatchloopshutdown"
#define YAF_PLUGIN_HOOK_PRERESPONSE             "preresponse"

namespace HPHP
{
    extern Variant yaf_dispatcher_dispatch(const Object* object);
    extern Variant yaf_dispatcher_instance(Object* object);
    extern int yaf_dispatcher_set_request(const Object* object, const Variant& request);
}


#endif //_YAF_DISPATCHER_H
