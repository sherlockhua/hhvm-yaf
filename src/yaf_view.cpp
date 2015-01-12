/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-12 11:52:36
*   description  ：
*
=============================================*/
#include "yaf_view.h"
#include "ext_yaf.h"


namespace HPHP {


void YafExtension::_initYafViewClass()
{
    _initYafViewInterfaceClass();
    _initYafViewSimpleClass();
}


}
