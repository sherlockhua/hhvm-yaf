/*===========================================
*   Copyright (C) 2014 All rights reserved.
*   
*   company      : xiaomi
*   author       : root
*   email        : root@xiaomi.com
*   date         ：2014-12-18 07:46:03
*   description  ：
*
=============================================*/
#ifndef _YAF_CONTROLLER_H_
#define _YAF_CONTROLLER_H_

namespace HPHP {

#define YAF_CONTROLLER_PROPERTY_NAME_MODULE     "_module"
#define YAF_CONTROLLER_PROPERTY_NAME_NAME       "_name"
#define YAF_CONTROLLER_PROPERTY_NAME_SCRIPT     "_script_path"
#define YAF_CONTROLLER_PROPERTY_NAME_RESPONSE   "_response"                                                       
#define YAF_CONTROLLER_PROPERTY_NAME_REQUEST    "_request"
#define YAF_CONTROLLER_PROPERTY_NAME_ARGS       "_invoke_args"
#define YAF_CONTROLLER_PROPERTY_NAME_ACTIONS    "actions"
#define YAF_CONTROLLER_PROPERTY_NAME_VIEW       "_view"

#define YAF_CONTROLLER_PROPERTY_NAME_RENDER     "yafAutoRender"


extern int yaf_controller_construct(Variant& control, const Object& request, 
        const Object& response, const Object& view, const Variant& params);

}



#endif //_YAF_CONTROLLER_H
