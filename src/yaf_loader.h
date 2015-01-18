/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-14 20:43:12
*   description  ：
*
=============================================*/
#ifndef _YAF_LOADER_H_
#define _YAF_LOADER_H_

#include "ext_yaf.h"

namespace HPHP {

extern Variant yaf_loader_import(const char *path, int len, int use_path);


}



#endif //_YAF_LOADER_H
