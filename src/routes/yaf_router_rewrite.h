/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-02 12:04:52
*   description  ：
*
=============================================*/
#ifndef _YAF_ROUTER_REWRITE_H_
#define _YAF_ROUTER_REWRITE_H_


namespace HPHP {

extern Variant yaf_route_rewrite_instance( const Object* object,
        const Variant& match, const Array& route, const Variant& verify);

}





#endif //_YAF_ROUTER_REWRITE_H
