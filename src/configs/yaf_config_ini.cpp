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
             line[indexj] == '\r'||
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

    auto ptr_config = o->o_realProp(YAF_CONFIG_PROPERT_NAME, 
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
    char line_size = sizeof(line);
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
            trim(ptr);
            if (strlen(ptr) == 0) {
                yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                        "invalid section:%s file:%s object:%p", 
                        line, filename, object);
                continue;
            }

            char* ptr_multi = strchr(ptr, ':');
            if (ptr_multi == NULL) {
                cur_section = std::string(ptr);
                if (config.exists(String(cur_section.c_str()))) {
                    yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                            "duplicate section:%s file:%s object:%p", 
                            line, filename, object);
                    fclose(fp);
                    return NULL;
                }

                Array tmp = Array::Create();
                config.set(String(cur_section.c_str()), tmp);
            } else {
                //inherit
                std::vector<std::string> vec = split(ptr, ':');
                if (vec.size() == 0) {
                    yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                            "invalid section:%s file:%s object:%p", 
                            line, filename, object);
                    continue;
                }

                char buf[8192];
                snprintf(buf, sizeof(buf), "%s", vec[0].c_str());
                trim(buf);

                cur_section = std::string(buf);
                if (config.exists(String(cur_section.c_str()))) {
                    yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                            "duplicate section:%s file:%s object:%p", 
                            line, filename, object);
                    fclose(fp);
                    return NULL;
                }

                Array tmp = Array::Create();
                for (int i = 1; i < vec.size(); i++) {
                    snprintf(buf, sizeof(buf), "%s", vec[0].c_str());
                    trim(buf);

                    if(!config.exists(String(buf))) {
                        yaf_trigger_error(YAF_ERR_TYPE_WARN, 
                                "section:%s file:%s object:%p not exists", 
                                line, filename, object);
        
                        continue;
                    }

                    tmp.set(String(buf), config[String[buf]]);   
                }
                config.set(String(cur_section.c_str()), tmp);
            }
        } else {
            //field parse
        }
    }

    fclose(fp);
    return 0;
}

#ifdef HHVM_VERSION_3_2_NEW
static Variant yaf_config_ini_instance(ObjectData* object, 
#else
static Variant yaf_config_ini_instance(Object* object, 
#endif
        const String& filename, const Variant& section)
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

    if (filename.isArray()) {
        auto ptr_config = o->o_realProp(YAF_CONFIG_PROPERT_NAME, 
                ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
        *ptr_config = filename;
        if (readonly.isBoolean()) {
            auto ptr_readonly = o->o_realProp(YAF_CONFIG_PROPERT_NAME_READONLY, 
                    ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
            *ptr_readonly = readonly.toBoolean();
        } 

        return o;
    }

    if(!filename.isString()) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, "invalid parameter provider,"\
                " expect a ini filename or array");
        return NULL;
    } 

    std::string str_filename = filename.toString().toCppString();
    struct stat buf;

    int ret = stat(filename.c_str(), &buf);
    if (ret != 0) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                "invalid parameter provider, stat failed," \
                " file:%s errno:%d", filename.c_str(), errno);
        return NULL;
    }
 
    if (!S_ISREG(buf.st_mode)) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                "expect filename to a regular file" \
                " file:%s errno:%d", filename.c_str(), errno);

        return NULL;
    }

    ret = parse_ini_file(filename.c_str(), o);
    if (ret != 0) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                "parse filename failed, file:%s ret:%d", 
                filename.c_str(), ret);
        return NULL;
    }

    return o;
}

static void HHVM_METHOD(Yaf_Config_Ini, __construct, 
        const String& filename, const Variant& section)
{
    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");

    if (!ptr_config->isArray()) {
        *ptr_config = Array::Create();
    }

    (void)yaf_config_ini_instance(&this_, filename, section);
}

void YafExtension::_initYafConfigIniClass()
{
    HHVM_ME(Yaf_Config_Ini, __construct);
}

}
