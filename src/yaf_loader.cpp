/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-14 20:42:47
*   description  ：
*
=============================================*/
#include "hphp/runtime/ext/std/ext_std_classobj.h"
#include "yaf_loader.h"
#include "ext_yaf.h"

namespace HPHP {

bool yaf_loader_register(const Object& loader) 
{
    Array params = Array::Create();
    Array real_params = Array::Create();

    String method(YAF_AUTOLOAD_FUNC_NAME);
    String function(YAF_SPL_AUTOLOAD_REGISTER_NAME);

    real_params.append(loader);
    real_params.append(method);

    params.append(real_params);

    Variant ret = vm_call_user_func(function, params);
    if (ret.toBoolean() == false) {
        raise_warning("Unable to register autoload function %s",
                YAF_AUTOLOAD_FUNC_NAME);
        return false;
    }

    return true;
}

static bool yaf_loader_is_category(const char* class_name, 
		unsigned int class_name_len, const char* category, unsigned int category_len)
{
	unsigned int separator_len = g_yaf_local_data.get()->name_separator.length();
	if (g_yaf_local_data.get()->name_suffix) {
		if (class_name_len > category_len && 
				strncmp(class_name + class_name_len - category_len, category, category_len) == 0) {
			int real_len = class_name_len - category_len - separator_len;
			if (!separator_len || strncmp(class_name + real_len, 
						g_yaf_local_data.get()->name_separator.c_str(), separator_len) == 0) {
				return true;
			}
		}
	} else {
		if (strncmp(class_name, category, category_len) == 0) {
			if (!separator_len||
					strncmp(class_name + category_len, 
						g_yaf_local_data.get()->name_separator.c_str(), separator_len) == 0) {
				return true;
			}
		}
	}

	return false;
}

bool yaf_loader_import(const char *path, int len, int use_path)
{ 
    String func("yaf_loader_import");

    Array params = Array::Create();
    params.append(String(std::string(path, len)));
    Variant ret = vm_call_user_func(func, params, true);
    return ret.toBoolean();
}

static Variant get_instance()
{
    Array func = Array::Create();
    func.append("Yaf_Loader");
    func.append("get_instance");

    Array params = Array::Create();
    return vm_call_user_func(func, params);
}

static void set_instance(const Object& object)
{ 
    Array func = Array::Create();
    func.append("Yaf_Loader");
    func.append("set_instance");

    Array params = Array::Create();
    params.append(object);
    vm_call_user_func(func, params);
}

Variant yaf_loader_instance(const Object* object, 
        const Variant& library_path, const Variant& global_path)
{
    Variant instance;
    instance = get_instance();
    if (instance.isObject()) {
    /* unecessary since there is no set_router things
       && instanceof_function(Z_OBJCE_P(instance), yaf_loader_ce TSRMLS_CC)) {
     */
        Object o_instance = instance.toObject();
        if (library_path.isString()) {
            auto ptr_library_path = o_instance->o_realProp(YAF_LOADER_PROPERTY_NAME_LIBRARY, 
                                        ObjectData::RealPropUnchecked, "Yaf_Loader");
            *ptr_library_path = library_path;
        }

        if (global_path.isString()) {
            auto ptr_global_path = o_instance->o_realProp(YAF_LOADER_PROPERTY_NAME_GLOBAL_LIB, 
                                        ObjectData::RealPropUnchecked, "Yaf_Loader");
            *ptr_global_path = global_path;
        }

        return instance;
    }

    if (global_path.isNull() && library_path.isNull()) {
        return init_null_variant;
    }

    if (object) {
        instance = *object;
    } else {
        instance = createObject("Yaf_Loader", Array::Create());
    }

    Object o_instance = instance.toObject();
    auto ptr_library_path = o_instance->o_realProp(YAF_LOADER_PROPERTY_NAME_LIBRARY, 
                                ObjectData::RealPropUnchecked, "Yaf_Loader");

    auto ptr_global_path = o_instance->o_realProp(YAF_LOADER_PROPERTY_NAME_GLOBAL_LIB, 
                                ObjectData::RealPropUnchecked, "Yaf_Loader");

    if (!library_path.isNull() && !global_path.isNull()) {
        *ptr_library_path = library_path;
        *ptr_global_path = global_path;
    } else if (global_path.isNull()) {
        *ptr_library_path = library_path;
        *ptr_global_path = library_path;
    } else {
        *ptr_library_path = global_path;
        *ptr_global_path = global_path;
    }

    if (!yaf_loader_register(o_instance)) {
        return init_null_variant;
    }

    set_instance(o_instance);
    return instance;
}

bool yaf_loader_register_namespace_single(const char *prefix, int len) 
{
    if (g_yaf_local_data.get()->local_namespaces.length()) {
        char buf[1024];
        snprintf(buf, sizeof(buf), "%c%s", DEFAULT_DIR_SEPARATOR, prefix); 
        g_yaf_local_data.get()->local_namespaces += buf; 
    } else {
        g_yaf_local_data.get()->local_namespaces = std::string(prefix);
    }   

    return true;
}

bool yaf_loader_register_namespace_multi(const Array& arr_namespace) 
{
    ArrayIter iter = arr_namespace.begin();
    while (!iter.end()) {
        Variant value = iter.second();
        if (value.isString()) {
            const char* prefix = value.toString().c_str();
            int prefix_len = strlen(prefix);

            yaf_loader_register_namespace_single(prefix, prefix_len);
        }

        iter.next();
    }

    return true;
}

bool yaf_loader_is_local_namespace(const Object& loader, char *class_name, int len) 
{
    char *pos, *prefix = NULL;
    char orig_char = 0, *backup = NULL;
    unsigned int prefix_len = 0;

    if (!g_yaf_local_data.get()->local_namespaces.length()) {
        return false;
    }

    char tmp_ns[8192];
    snprintf(tmp_ns, sizeof(tmp_ns), "%s", g_yaf_local_data.get()->local_namespaces.c_str());

    char* ns = tmp_ns;
    pos = strstr(class_name, "_");
    if (pos) {
        prefix_len  = pos - class_name;
        prefix      = class_name;
        backup = class_name + prefix_len;
        orig_char = '_';
        *backup = '\0';
    }
#ifdef YAF_HAVE_NAMESPACE
    else if ((pos = strstr(class_name, "\\"))) {
        prefix_len  = pos - class_name;
        prefix      = class_name;
        orig_char = '\\';
        backup = class_name + prefix_len;
        *backup = '\0';
    }
#endif
    else {
        prefix = class_name;
        prefix_len = len;
    }

    if (!prefix) {
        return false;
    }

    while ((pos = strstr(ns, prefix))) {
        if ((pos == ns) && (*(pos + prefix_len) == DEFAULT_DIR_SEPARATOR || *(pos + prefix_len) == '\0')) {
            if (backup) {
                *backup = orig_char;
            }
            return true;
        } else if (*(pos - 1) == DEFAULT_DIR_SEPARATOR 
                && (*(pos + prefix_len) == DEFAULT_DIR_SEPARATOR || *(pos + prefix_len) == '\0')) {
            if (backup) {
                *backup = orig_char;
            }
            return true;
        }
        ns = pos + prefix_len;
    }

    if (backup) {
        *backup = orig_char;
    }

    return false;
}

int yaf_internal_autoload(const char* ptr_file_name, 
        int ptr_file_name_len, std::string& directory)
{
    char *q, *p;
    unsigned int seg_len, directory_len;
    std::string buf;

    char file_name[8192];
    std::string tmp_str(ptr_file_name, ptr_file_name_len);

    snprintf(file_name, sizeof(file_name), "%s", tmp_str.c_str());
    int file_name_len = strlen(file_name);

    if (directory.length() == 0) {
        String library_path;
        Variant loader;

        loader = yaf_loader_instance(NULL, init_null_variant, init_null_variant);

        if (loader.isNull()) {
            raise_warning("yaf_loader need to be initialize first");
            return HHVM_YAF_FAILED;
        } else {
            Object o_loader = loader.toObject();

            auto ptr_library_dir = o_loader->o_realProp(YAF_LOADER_PROPERTY_NAME_LIBRARY,
                    ObjectData::RealPropUnchecked, "Yaf_Loader");

            auto ptr_global_dir = o_loader->o_realProp(YAF_LOADER_PROPERTY_NAME_GLOBAL_LIB,
                    ObjectData::RealPropUnchecked, "Yaf_Loader");

            if (yaf_loader_is_local_namespace(o_loader, file_name, file_name_len)) {
                library_path = ptr_library_dir->toString();
            } else {
                library_path = ptr_global_dir->toString();
            }
        }

        if (library_path.length() == 0) {
            raise_warning("yaf_loader requires yaf_application(which set the library_directory) "\
                    "to be initialized first");
            return HHVM_YAF_FAILED;
        }

        buf.append(library_path.c_str());
    } else {
        buf.append(directory);
    }

    directory_len = buf.length();

    /* aussume all the path is not end in slash */
    buf.append(DEFAULT_SLASH_STR);
    p = file_name;
    q = p;

    while (1) {
        while(++q && *q != '_' && *q != '\0');

        if (*q != '\0') {
            seg_len = q - p;
            if (seg_len) {
                buf.append(std::string(p, seg_len));
                buf.append(DEFAULT_SLASH_STR);
            }
            p       = q + 1;
        } else {
            break;
        }
    }

    if (g_yaf_local_data.get()->lowcase_path) {
        /* all path of library is lowercase */
        transform(buf.begin() + directory_len, buf.end(), 
                buf.begin() + directory_len, tolower);
    }

    buf.append(std::string(p, strlen(p)));
    buf.append(".");
    buf.append(g_yaf_local_data.get()->ext);

    directory = buf;
    bool status = yaf_loader_import(buf.c_str(), buf.length(), 0);
    if (!status) {
        return HHVM_YAF_FAILED;
    }

    return HHVM_YAF_SUCCESS;
}

static void HHVM_METHOD(Yaf_Loader, __clone)
{
}

static void HHVM_METHOD(Yaf_Loader, __construct)
{
}

static void HHVM_METHOD(Yaf_Loader, __sleep)
{
}

static void HHVM_METHOD(Yaf_Loader, __wakeup)
{
}

static Variant HHVM_METHOD(Yaf_Loader, autoload, const String& str_class_name)
{
	const char* app_directory = g_yaf_local_data.get()->directory.c_str();
	unsigned int separator_len = g_yaf_local_data.get()->name_separator.length();
	const char* origin_classname = str_class_name.c_str();
	const char* class_name = origin_classname;

    std::string str_directory;
    char directory[8192] = {'\0'};
    char file_name[8192];
    int file_name_len = 0;

	size_t class_name_len = strlen(class_name);
#ifdef YAF_HAVE_NAMESPACE
	char* origin_lcname = strdup(origin_classname);
	class_name = origin_lcname;
	int pos = 0;
	while (pos < class_name_len) {
		if (class_name[pos] == '\\') {
			class_name[pos] = '_';
		}
		pos++;
	}
#endif

	if (strncmp(class_name, YAF_LOADER_RESERVERD, YAF_LOADER_LEN_RESERVERD) == 0) {
		raise_error("You should not use '%s' as class name prefix", YAF_LOADER_RESERVERD);
		return init_null_variant;
	}

		
	if (yaf_loader_is_category(class_name, class_name_len, 
				YAF_LOADER_MODEL,YAF_LOADER_LEN_MODEL )) {
        snprintf(directory, sizeof(directory), "%s/%s", 
                app_directory, YAF_MODEL_DIRECTORY_NAME);
        file_name_len = class_name_len - separator_len - YAF_LOADER_LEN_MODEL;
        if (g_yaf_local_data.get()->name_suffix) {
            strncpy(file_name, class_name, file_name_len);
        } else {
            strcpy(file_name, class_name + YAF_LOADER_LEN_MODEL + separator_len);
        }

        goto found;
	}

    if (yaf_loader_is_category(class_name, class_name_len, 
                YAF_LOADER_PLUGIN, YAF_LOADER_LEN_PLUGIN)) {
         snprintf(directory, sizeof(directory), "%s/%s", 
                app_directory, YAF_PLUGIN_DIRECTORY_NAME);
        file_name_len = class_name_len - separator_len - YAF_LOADER_LEN_PLUGIN;
        if (g_yaf_local_data.get()->name_suffix) {
            strncpy(file_name, class_name, file_name_len);
        } else {
            strcpy(file_name, class_name + YAF_LOADER_LEN_PLUGIN + separator_len);
        }

        goto found;
    }

    if (yaf_loader_is_category(class_name, class_name_len, 
                YAF_LOADER_CONTROLLER, YAF_LOADER_LEN_CONTROLLER)) {
         snprintf(directory, sizeof(directory), "%s/%s", 
                app_directory, YAF_CONTROLLER_DIRECTORY_NAME);
        file_name_len = class_name_len - separator_len - YAF_LOADER_LEN_CONTROLLER;
        if (g_yaf_local_data.get()->name_suffix) {
            strncpy(file_name, class_name, file_name_len);
        } else {
            strcpy(file_name, class_name + YAF_LOADER_LEN_CONTROLLER + separator_len);
        }

        goto found;
    }

    if (g_yaf_local_data.get()->st_compatible) {
        if (strncmp(class_name, YAF_LOADER_DAO, YAF_LOADER_LEN_DAO) == 0 ||
            strncmp(class_name, YAF_LOADER_SERVICE, YAF_LOADER_LEN_SERVICE) == 0) {
        
            snprintf(directory, sizeof(directory), "%s/%s", 
                    app_directory, YAF_MODEL_DIRECTORY_NAME);
        }
    }

    file_name_len = class_name_len;
    strncpy(file_name, class_name, class_name_len);

found:
    if (g_yaf_local_data.get()->directory.length() == 0 &&
            strlen(directory)) {
#ifdef YAF_HAVE_NAMESPACE
        if (origin_lcname) {
            free(origin_lcname);
        }
#endif
        raise_warning("Couldn't load a framework MVC class without "\
                "an Yaf_Application initializing");
        return false;
    }

    str_directory = directory;
    if (yaf_internal_autoload(file_name, file_name_len, str_directory) == HHVM_YAF_SUCCESS) {

        std::string tmp_class_name(origin_classname);
        String str_class_name(tmp_class_name);

        if (HHVM_FN(class_exists)(str_class_name) || 
                HHVM_FN(interface_exists)(str_class_name)) {
#ifdef YAF_HAVE_NAMESPACE
            if (origin_lcname) {
                free(origin_lcname);
            }
#endif
            return true;
        } 

        if (!g_yaf_local_data.get()->use_spl_autoload) {
            raise_error("count not found class %s in directory:%s", 
                    class_name, str_directory.c_str());
            return true;
        }

        return false;
    } 

#ifdef YAF_HAVE_NAMESPACE
    if (origin_lcname) {
        free(origin_lcname);
    }
#endif

    if (!g_yaf_local_data.get()->use_spl_autoload) {
        raise_warning("Failed opening script %s: %s", 
                str_directory.c_str(), strerror(errno));
        return true;
    }

    return false;
}

static Variant HHVM_STATIC_METHOD(Yaf_Loader, getInstance, 
        const Variant& library_path, const Variant& global_path)
{
    return yaf_loader_instance(NULL, library_path, global_path);    
}

static Variant HHVM_METHOD(Yaf_Loader, registerLocalNamespace, const Variant& var_namespace)
{
    if (var_namespace.isString()) {
        const char* prefix = var_namespace.toString().c_str();
        int prefix_length = strlen(prefix);
        if (yaf_loader_register_namespace_single(prefix, prefix_length)) {
            return this_;
        }

        return false;
    }

    if (var_namespace.isArray()) {
        if (yaf_loader_register_namespace_multi(var_namespace.toArray())) {
            return this_;
        }

        return false;
    }

    return false;
}

static Variant HHVM_METHOD(Yaf_Loader, getLocalNamespace)
{
    if (g_yaf_local_data.get()->local_namespaces.length()) {
        return String(g_yaf_local_data.get()->local_namespaces);
    }

    return init_null_variant;
}

static Variant HHVM_METHOD(Yaf_Loader, clearLocalNamespace)
{
    if (g_yaf_local_data.get()->local_namespaces.length()) {
        g_yaf_local_data.get()->local_namespaces.clear();
    }

    return true;
}

static Variant HHVM_METHOD(Yaf_Loader, isLocalName, const Variant& name)
{
    if (name.isString()) {
        char class_name[8192];
        snprintf(class_name, sizeof(class_name), "%s", name.toString().c_str());
        int class_name_len = strlen(class_name);

        return yaf_loader_is_local_namespace(this_, class_name, class_name_len);
    }

    return false;
}

static Variant HHVM_STATIC_METHOD(Yaf_Loader, import, 
        const String& file_path)
{
    if (!file_path.length()) {
        return false;
    }

    char filename[8192];
    snprintf(filename, sizeof(filename), "%s", file_path.c_str());

    if (!IS_ABSOLUTE_PATH(file_path)) {
        Variant var_loader = yaf_loader_instance(NULL, init_null_variant, 
                init_null_variant);
        if (var_loader.isNull()) {
            raise_warning("Yaf_Loader need to be initialize first");
            return false;
        }

        Object o_loader = var_loader.toObject();
        auto ptr_library = o_loader->o_realProp(YAF_LOADER_PROPERTY_NAME_LIBRARY, 
                            ObjectData::RealPropUnchecked, "Yaf_Loader");

        snprintf(filename, sizeof(filename), "%s%c%s",
                ptr_library->toString().c_str(), DEFAULT_SLASH_CHAR, file_path.c_str());
    }

    bool ret = yaf_loader_import(filename, strlen(filename), 0);
    return ret;
}

static Variant HHVM_METHOD(Yaf_Loader, setLibraryPath, 
        const String& path, const Variant& is_global)
{
    bool global = false;
    if (is_global.isBoolean()) {
        global = is_global.toBoolean();
    }
    
    if (global) {
        auto ptr_library = this_->o_realProp(YAF_LOADER_PROPERTY_NAME_GLOBAL_LIB, 
                            ObjectData::RealPropUnchecked, "Yaf_Loader");
        *ptr_library = path;
    } else {
        auto ptr_library = this_->o_realProp(YAF_LOADER_PROPERTY_NAME_LIBRARY, 
                            ObjectData::RealPropUnchecked, "Yaf_Loader");
        *ptr_library = path;
    }

    return this_;
}

static Variant HHVM_METHOD(Yaf_Loader, getLibraryPath, 
        const Variant& is_global)
{
    bool global = false;
    if (is_global.isBoolean()) {
        global = is_global.toBoolean();
    }
    
    if (global) {
        auto ptr_library = this_->o_realProp(YAF_LOADER_PROPERTY_NAME_GLOBAL_LIB, 
                            ObjectData::RealPropUnchecked, "Yaf_Loader");
        return *ptr_library;
    } else {
        auto ptr_library = this_->o_realProp(YAF_LOADER_PROPERTY_NAME_LIBRARY, 
                            ObjectData::RealPropUnchecked, "Yaf_Loader");
        return *ptr_library;
    }

    return init_null_variant;
}



void YafExtension::_initYafAutoloadClass()
{
    HHVM_ME(Yaf_Loader, __clone);
    HHVM_ME(Yaf_Loader, __construct);
    HHVM_ME(Yaf_Loader, __sleep);
    HHVM_ME(Yaf_Loader, __wakeup);

    HHVM_ME(Yaf_Loader, autoload);
    HHVM_STATIC_ME(Yaf_Loader, getInstance);
    HHVM_ME(Yaf_Loader, registerLocalNamespace);
    HHVM_ME(Yaf_Loader, getLocalNamespace);

    HHVM_ME(Yaf_Loader, clearLocalNamespace);
    HHVM_ME(Yaf_Loader, isLocalName);
    HHVM_STATIC_ME(Yaf_Loader, import);

    HHVM_ME(Yaf_Loader, setLibraryPath);
    HHVM_ME(Yaf_Loader, getLibraryPath);
}


}
