/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-22 16:40:02
*   description  ：
*
=============================================*/
#include "yaf_config.h"
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/ext/extension.h"
#include "ext_yaf.h"

namespace HPHP {

static void HHVM_METHOD(Yaf_Config_Abstract, __construct)
{}


void YafExtension::_initYafConfigClass()
{
    HHVM_ME(Yaf_Config_Abstract, __construct);

    _initYafConfigSimpleClass();
    _initYafConfigIniClass();
}

void yaf_config_cursor::sweep() {
  Close();
}



}
