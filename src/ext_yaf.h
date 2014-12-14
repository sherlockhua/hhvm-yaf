/*===========================================
*   Copyright (C) 2014 All rights reserved.
*   
*   company      : xiaomi
*   author       : root
*   email        : root@xiaomi.com
*   date         ：2014-12-13 11:11:10
*   description  ：
*
=============================================*/
#ifndef _EXT_EXAMPLE_H_
#define _EXT_EXAMPLE_H_

namespace HPHP {

class YafExtension : public Extension {
public:
    YafExtension():Extension("yaf"){}
    virtual void moduleInit();
    void moduleLoad(Hdf config);
    void moduleLoad(const IniSetting::Map& ini, Hdf config);

private:
    void _loadYafConf(Hdf conf);
    void _initYafActionClass();
    void _initYafControllerClass();


}; 

class YafRequestData: public RequestEventHandler {

public: 
    char        *ext; 
    char        *base_uri;
    char        *environ;
    char        *directory;
    char        *local_library;
    char        *local_namespaces;
    char        *global_library;
    char        *view_directory;
    char        *view_ext;
    char        *default_module;
    char        *default_controller;
    char        *default_action;
    char        *bootstrap;
    char        *name_separator;
    long        name_separator_len;
    bool   lowcase_path;
    bool   use_spl_autoload;
    bool   throw_exception;
    bool   cache_config;
    bool   action_prefer;
    bool   name_suffix;
    bool   autoload_started;
    bool   running;
    bool   in_exception;
    bool   catch_exception;
    bool   suppressing_warning;
    /* {{{ This only effects internally */
    bool   st_compatible;
    /* }}} */
    long        forward_limit;
    Array       *configs;
    Variant     *modules;
    Variant     *default_route;
/*#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION < 4))
    uint        buf_nesting;
    void        *buffer;
    void        *owrite_handler;
#endif
*/
    Variant     *active_ini_file_section;
    Variant     *ini_wanted_section;
    int         parsing_flag;
    bool        use_namespace;

public:
        virtual void requestInit() {
            this->running          = 0;
            this->in_exception     = 0;
            this->throw_exception  = 1;
            this->catch_exception  = 0;
            this->directory        = NULL;
            this->bootstrap        = NULL;
            this->local_library    = NULL;
            this->local_namespaces = NULL;
            this->modules          = NULL;
            this->base_uri         = NULL;
            this->view_directory   = NULL;
/*
#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION < 4))
            this->buffer           = NULL;
            this->owrite_handler   = NULL;
            this->buf_nesting      = 0;
#endif
*/
        }

        virtual void requestShutdown() {
        }
};


}


#endif //_EXT_EXAMPLE_H

