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
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/ext/extension.h"
#include "ext_yaf.h"

namespace HPHP { 

static int64_t HHVM_METHOD(calc, add, int64_t a, int64_t b)
{
    return a + b;
}

static void HHVM_METHOD(calc, __construct)
{

}

static void HHVM_METHOD(Yaf_Action_Abstract, __construct)
{

}

static Variant HHVM_METHOD(Yaf_Action_Abstract, getController)
{
    return 100033;
}

static Variant HHVM_METHOD(Yaf_Action_Abstract, execute)
{
    return false;
}

static Variant HHVM_METHOD(Yaf_Controller, test)
{
    return String("this is in cpp");
}

class yafExtension : public Extension {
public:
    yafExtension():Extension("yaf"){}

    virtual void moduleInit()
    {
        HHVM_ME(calc, add);
        HHVM_ME(calc, __construct);

        //HHVM_ME(Yaf_Action_Abstract, execute);
        HHVM_ME(Yaf_Action_Abstract, getController);
        HHVM_ME(Yaf_Action_Abstract, __construct);

        HHVM_ME(Yaf_Controller, test);
        loadSystemlib();
    }
} s_yaf_extension;

HHVM_GET_MODULE(yaf);
}
