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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//#include "ext_example.h"
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/ext/extension.h"
#include "ext_yaf.h"
#include "yaf_config.h"

namespace HPHP { 

static std::vector<std::string> split(const char* line, char demi)
{
    std::vector<std::string> vec;
    int len = strlen(line);
    int i = 0;
    std::string field;
    for (; i < len; i++) {
        if (line[i] == demi){
            if (field.length()) {
                vec.push_back(field);
            }
            field.clear();
            continue;
        }

        field.push_back(line[i]);
    }

    if (field.length()) {
        vec.push_back(field);
    }

    return vec;
}

static char* trim(char* line)
{
    if (line == NULL) {
        return line;
    }

    int len = strlen(line);
    if (len == 0) {
        return line;
    }

    while (len && 
            (line[len - 1] == ' ' || 
             line[len - 1] == '\t'||
             line[len - 1] == '\r'||
             line[len - 1] == '\n'||
             line[len - 1] == '\b')) {
        line[len -1] = '\0';
        len--;
    }

    if (len == 0) {
        return line;
    }

    int index = 0;
    while (index < len && 
            (line[index] == ' ' || 
             line[index] == '\t'||
             line[index] == '\r'||
             line[index] == '\n'||
             line[index] == '\b')) {
        index++;
    }

    if (index >= len) {
        line[0] = '\0';
        return line;
    }
    
    if (index == 0) {
        return line;
    }

    memmove(line, &line[index], len - index);
    return line;
}

static int build_array(Array& arr, std::vector<std::string>& vec,
        const std::string& value)
{
    if (vec.size() == 0) {
        return 0;
    }

    std::string section = *vec.begin();
    vec.erase(vec.begin());

    if (arr.exists(String(section))) {
        Variant& cur_var = arr.lvalAt(String(section));
        Array& cur = cur_var.toArrRef();

        build_array(cur, vec, value);
    } else {
        if (vec.size() == 0) {
            arr.set(String(section), value);
        } else {
            arr.set(String(section), Array::Create());

            Variant& cur_var = arr.lvalAt(String(section));
            Array& cur = cur_var.toArrRef();
            return build_array(cur, vec, value);
        }
    }

    return 0;
}

static int parse_field(char* field, Array& config, const char* cur_section, const char* filename)
{
    if (field == NULL || filename == NULL || cur_section == NULL) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                "invalid parameter:%p file:%s", 
                field, filename);
        return -1;
    }

    Array* ptr_array = &config;
    if (strlen(cur_section)) {
        if (!config.exists(String(cur_section))) {
            Array tmp = Array::Create();
            config.set(String(cur_section), tmp);
        } 

        ptr_array = &(config.lvalAt(String(cur_section)).toArrRef());
    }

    //Array& cur = cur_var.toArrRef();

    char* ptr = field;
    trim(ptr);
    if (strlen(ptr) == 0) {
        return 0;
    }

    std::vector<std::string> vec = split(ptr, '=');
    if (vec.size() != 2) {
        yaf_trigger_error(YAF_ERR_TYPE_WARN, 
                "invalid parameter:%p file:%s", 
                field, filename);
        return 0;
    }

    char key[8192];
    char value[8192];

    snprintf(key, sizeof(key), "%s", vec[0].c_str());
    snprintf(value, sizeof(value), "%s", vec[1].c_str());

    trim(key);
    trim(value);

    if (strlen(key) == 0) {
        yaf_trigger_error(YAF_ERR_TYPE_WARN, 
                "invalid key:%p file:%s", 
                field, filename);
        return 0;
    }

    vec = split(key, '.');
    if (vec.size() == 1) {
        ptr_array->set(String(key), String(value));
    } else {
        build_array(*ptr_array, vec, std::string(value));
    }

    return 0;
}

static int parse_section(char* line, Array& config, const char* filename, std::string& cur_section)
{
    if (line == NULL || filename == NULL) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                "invalid parameter:%p file:%s", 
                line, filename);
        return -1;
    }

    char* ptr = line;
    trim(ptr);
    if (strlen(ptr) == 0) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                "invalid section:%s file:%s", 
                line, filename);
        return -1;
    }

    char* ptr_multi = strchr(ptr, ':');
    if (ptr_multi == NULL) {
        cur_section = std::string(ptr);
        if (config.exists(String(cur_section.c_str()))) {
            yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                    "duplicate section:%s file:%s", 
                    line, filename);
            return -2;
        }

        Array tmp = Array::Create();
        config.set(String(cur_section.c_str()), tmp);
    } else {
        //inherit
        std::vector<std::string> vec = split(ptr, ':');
        if (vec.size() == 0) {
            yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                    "invalid section:%s file:%s", 
                    line, filename);
            return -3;
        }

        char buf[8192];
        snprintf(buf, sizeof(buf), "%s", vec[0].c_str());
        trim(buf);

        cur_section = std::string(buf);
        if (config.exists(String(cur_section.c_str()))) {
            yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                    "duplicate section:%s file:%s", 
                    line, filename);
            return -4;
        }

        Array tmp = Array::Create();
        for (int i = 1; i < vec.size(); i++) {
            snprintf(buf, sizeof(buf), "%s", vec[0].c_str());
            trim(buf);

            if(!config.exists(String(buf))) {
                yaf_trigger_error(YAF_ERR_TYPE_WARN, 
                        "section:%s file:%s  not exists", 
                        line, filename);

                continue;
            }

            tmp.set(String(buf), config[String(buf)]);   
        }
        config.set(String(cur_section.c_str()), tmp);
    }

    return 0;
}

#ifdef HHVM_VERSION_3_2_NEW
static  int parse_ini_file(const char* filename, ObjectData* object)
#else 
static  int parse_ini_file(const char* filename, Object* object)
#endif
{
    if (filename == NULL || object == NULL) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                "invald file:%s object:%p", 
                filename, object);
        return -1;
    }

    auto ptr_config = (*object)->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
    Array& config = ptr_config->toArrRef();

    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                "open file:%s failed, object:%p, err:%d", 
                filename, object, errno);
        return -2;
    }

    char line[8192];
    int line_size = sizeof(line);
    std::string cur_section;

    while (fgets(line, line_size, fp)) {
        trim(line);
        int len = strlen(line);
        if (len == 0) {
            continue;
        }

        if (line [0] == '#' || line [0] == ';') {
            continue;
        }

        if (line[0] == '[') {
            if (len <= 2) {
                yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                        "invalid section:%s file:%s object:%p", 
                        line, filename, object);
                continue;
            }
            if (line[len - 1] != ']') {
                yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                        "invalid section:%s file:%s object:%p", 
                        line, filename, object);
                continue;
            }

            line[len - 1] = '\0';
            char* ptr = line+1;
            int ret = parse_section(ptr, config, filename, cur_section);
            if (ret != 0) {
                yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                        "parse section:%s failed file:%s object:%p", 
                        line, filename, object);
                fclose(fp);
                return ret;
            }
        } else {
            //field parse
            int ret = parse_field(line, config, cur_section.c_str(), filename);
            if (ret != 0) {
                yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                        "parse field:%s failed, cur_sectioin:%s file:%s object:%p", 
                        line, cur_section.c_str(), filename, object);
                fclose(fp);
                return ret;
            }
        }
    }

    fclose(fp);
    return 0;
}

#ifdef HHVM_VERSION_3_2_NEW
static Variant yaf_config_ini_instance(ObjectData* object, 
#else
static Variant yaf_config_ini_instance(const Object* object, 
#endif
        const Variant& filename, const Variant& section)
{
    Object o;
    if (object == NULL) {
        Array arr = Array::Create();
        arr.append(filename);
        arr.append(section);

        o = createObject("Yaf_Config_Ini", arr) ;
    } else {
        o = *object;
    }

    auto ptr_config = o->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");


    if (filename.isArray()) {
        *ptr_config = filename;
        return o;
    }

    if(!filename.isString()) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, "invalid parameter provider,"\
                " expect a ini filename or array");
        return init_null_variant;
    } 

    std::string str_filename = filename.toString().toCppString();
    struct stat buf;

    int ret = stat(str_filename.c_str(), &buf);
    if (ret != 0) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                "invalid parameter provider, stat failed," \
                " file:%s errno:%d", str_filename.c_str(), errno);
        return init_null_variant;
    }
 
    if (!S_ISREG(buf.st_mode)) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                "expect filename to a regular file" \
                " file:%s errno:%d", str_filename.c_str(), errno);
        return init_null_variant;
    }

    ret = parse_ini_file(str_filename.c_str(), &o);
    if (ret != 0) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                "parse filename failed, file:%s ret:%d", 
                str_filename.c_str(), ret);
        return init_null_variant;
    }

    return o;
}

static void HHVM_METHOD(Yaf_Config_Ini, __construct, 
        const Variant& filename, const Variant& section)
{
    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");

    if (!ptr_config->isArray()) {
        *ptr_config = Array::Create();
    }

    (void)yaf_config_ini_instance(&this_, filename, section);
}

static Variant HHVM_METHOD(Yaf_Config_Ini, get, const Variant& name)
{
    if (!name.isString() || (name.toString().length() == 0)) {
        return this_;
    }

    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");

    if (!ptr_config->isArray()) {
        *ptr_config = Array::Create();
        return false;
    } 

    Array& arr = ptr_config->toArrRef();
    if (!arr.exists(name.toString())) {
        return false;
    }

    Variant value = arr[name.toString()];
    return value;
    /*
    if (value.isArray()) {
        Variant instance = yaf_config_simple_format(&this_, value);
        return instance;
    }*/

    return value;
}



void YafExtension::_initYafConfigIniClass()
{
    HHVM_ME(Yaf_Config_Ini, __construct);
    HHVM_ME(Yaf_Config_Ini, get);
}

}
