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
//#include "ext_example.h"

#include <iostream>  
#include <algorithm> 

#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/ext/extension.h"
#include "ext_yaf.h"

namespace HPHP { 


static Variant HHVM_METHOD(Yaf_Controller_Abstract, test)
{
    return String("this is in cpp test!!!");
}

static Variant HHVM_METHOD(Yaf_Controller_Abstract, render, const String& tpl, 
        const Array& parameters)
{
    /*
    public    $actions      = NULL;
    protected $_module      = NULL;
    protected $_name        = NULL;
    protected $_request     = NULL;
    protected $_response    = NULL;
    protected $_invoke_args = NULL;
    protected $_view        = NULL;
    */
    auto _viewTmp = this_->o_realProp("_view", 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract");
    if (_viewTmp->isNull()) {
        return false;
    }

    auto _view = _viewTmp->toObject();
    std::string _name = this_->o_realProp("_name", 
            ObjectData::RealPropUnchecked, "Yaf_Controller_Abstract")->toString().toCppString();

    std::string view_ext = g_yaf_local_data.get()->view_ext;
    std::string self_name;
    transform(_name.begin(), _name.end(), self_name.begin(), tolower);

    for (unsigned int i = 0; i < self_name.length(); i++) {
        if (self_name[i] == '_') {
            self_name[i] = DEFAULT_SLASH_CHAR;
        }
    }

    std::string action_name = tpl.toCppString();

    for (unsigned int i = 0; i < action_name.length(); i++) {
        if (action_name[i] == '_') {
            action_name[i] = DEFAULT_SLASH_CHAR;
        }
    }

    std::string path = self_name  + DEFAULT_SLASH_STR + action_name + 
                        "." + view_ext;

    Array func = Array::Create();
    func.append(_view);
    func.append(String("render"));

    Array params = Array::Create();
    params.append(String(path.c_str()));

    if (!parameters.isNull()) {
        params.append(parameters);
    }

    Variant ret = vm_call_user_func(func, params);
    if (ret.isBoolean() && !ret.toBoolean()) {
        return false;
    }

    return ret;
}

void YafExtension::_initYafControllerClass()
{
    HHVM_ME(Yaf_Controller_Abstract, render);
    HHVM_ME(Yaf_Controller_Abstract, test);
}

}
