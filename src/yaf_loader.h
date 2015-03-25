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

#define YAF_DEFAULT_VIEW_EXT                "phtml"
#define YAF_DEFAULT_LIBRARY_EXT             YAF_DEFAULT_CONTROLLER_EXT

#define YAF_LIBRARY_DIRECTORY_NAME          "library"
#define YAF_CONTROLLER_DIRECTORY_NAME       "controllers"
#define YAF_PLUGIN_DIRECTORY_NAME           "plugins"
#define YAF_MODULE_DIRECTORY_NAME           "modules"
#define YAF_VIEW_DIRECTORY_NAME             "views"
#define YAF_MODEL_DIRECTORY_NAME            "models"

#define YAF_SPL_AUTOLOAD_REGISTER_NAME      "spl_autoload_register"
#define YAF_AUTOLOAD_FUNC_NAME              "autoload"
#define YAF_LOADER_PROPERTY_NAME_INSTANCE   "_instance"
#define YAF_LOADER_PROPERTY_NAME_NAMESPACE  "_local_ns"

#define YAF_LOADER_CONTROLLER               "Controller"
#define YAF_LOADER_LEN_CONTROLLER           10
#define YAF_LOADER_MODEL                    "Model"
#define YAF_LOADER_LEN_MODEL                5
#define YAF_LOADER_PLUGIN                   "Plugin"
#define YAF_LOADER_LEN_PLUGIN               6
#define YAF_LOADER_RESERVERD                "Yaf_"
#define YAF_LOADER_LEN_RESERVERD            3

/* {{{ This only effects internally */
#define YAF_LOADER_DAO                      "Dao_"
#define YAF_LOADER_LEN_DAO                  4
#define YAF_LOADER_SERVICE                  "Service_"
#define YAF_LOADER_LEN_SERVICE              8

#define YAF_LOADER_PROPERTY_NAME_LIBRARY    "_library"      
#define YAF_LOADER_PROPERTY_NAME_GLOBAL_LIB "_global_library"


extern bool yaf_loader_import(const char *path, int len, int use_path);
extern int yaf_loader_register_namespace_single(const std::string& str_namespace);
extern Variant yaf_loader_instance(const Object* object, 
        const Variant& local_library, const Variant& var_global_library);

//extern int yaf_internal_autoload(const char* file_name, int len, char** directory);
extern int yaf_internal_autoload(const char* file_name, 
        int len, std::string& directory);
}



#endif //_YAF_LOADER_H
