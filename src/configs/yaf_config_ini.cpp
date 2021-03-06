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

#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/ext/extension.h"
#include "ext_yaf.h"
#include "yaf_config.h"

#define HHVM_YAF_BUF_NOT_ENOUGH 1005

#define HHVM_YAF_ITEM_TYPE_ARRAY 1
#define HHVM_YAF_ITEM_TYPE_ENTRY 2
#define HHVM_YAF_ITEM_TYPE_STR   3

#define HHVM_YAF_PACK_TYPE(config, type)                           \
    do {                                                           \
        if (config.write_pos + sizeof(type) > config.data_size){   \
            return HHVM_YAF_BUF_NOT_ENOUGH;                        \
        }                                                          \
        ((config).data[(config).write_pos++]) = type;              \
    } while(0)

#define HHVM_YAF_PACK_INT(config, value)                           \
    do {                                                           \
        if (config.write_pos + sizeof(value)> config.data_size){   \
            return HHVM_YAF_BUF_NOT_ENOUGH;                        \
        }                                                          \
                                                                   \
        *((int*)&((config).data[(config).write_pos])) = value;     \
        (config).write_pos += sizeof(int);                         \
    } while(0)

#define HHVM_YAF_PACK_STR(config, value)                               \
    do {                                                               \
        if (config.write_pos + strlen(value)> config.data_size){       \
            return HHVM_YAF_BUF_NOT_ENOUGH;                            \
        }                                                              \
                                                                       \
        *((int*)&((config).data[(config).write_pos])) = strlen(value); \
        (config).write_pos += sizeof(int);                             \
        memcpy((char*)&(config).data[(config).write_pos],              \
                value, strlen(value));                                 \
        (config).write_pos += strlen(value);                           \
    } while(0)

#define HHVM_YAF_UNPACK_TYPE(config, type)                             \
    do {                                                               \
        if (config.read_pos > config.data_size) {                      \
            return HHVM_YAF_BUF_NOT_ENOUGH;                            \
        }                                                              \
                                                                       \
        (type) = ((config).data[(config).read_pos]);                   \
        (config).read_pos++;                                           \
    } while(0)

#define HHVM_YAF_UNPACK_INT(config, value)                             \
    do {                                                               \
        if (config.read_pos + sizeof(value)> config.data_size){        \
            return HHVM_YAF_BUF_NOT_ENOUGH;                            \
        }                                                              \
                                                                       \
        value = *((int*)&((config).data[(config).read_pos]));          \
        (config).read_pos += sizeof(int);                              \
    } while(0)

#define HHVM_YAF_UNPACK_STR(config, value, size)                     \
    do {                                                             \
        if (config.read_pos + sizeof(int)> config.data_size){       \
            return HHVM_YAF_BUF_NOT_ENOUGH;                          \
        }                                                            \
                                                                     \
        int length = *((int*)&((config).data[(config).read_pos]));   \
        (config).read_pos += sizeof(int);                            \
        if (length >= size){                                         \
            return HHVM_YAF_BUF_NOT_ENOUGH;                          \
        }                                                            \
        memcpy(value, (char*)&(config).data[(config).read_pos],      \
                length);                                             \
        value[length] = '\0';                                        \
        (config).read_pos += length;                                 \
    } while(0)


namespace HPHP { 

static void trim(std::string& str)
{
    std::string::size_type pos = str.find_last_not_of(" \t\r\n");
    if (pos != std::string::npos) {
        str.erase(pos+1);
    } else {
        str.clear();
    }

    pos = str.find_first_not_of(" \t\r\n");
    if (pos != std::string::npos) {
        str.erase(0, pos);
    }
}

static std::vector<std::string> split(const char* line, char demi)
{
    std::vector<std::string> vec;
    int len = strlen(line);
    int i = 0;
    std::string field;
    for (; i < len; i++) {
        if (line[i] == demi){
            trim(field);
            if (field.length()) {
                vec.push_back(field);
            }
            field.clear();
            continue;
        }

        field.push_back(line[i]);
    }

    trim(field);
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

    if (line[index] == '\"' && line[len - 1] == '\"') {
        index++;
        len--;
    }

    if (line[index] == '\'' && line[len - 1] == '\'') {
        index++;
        len--;
    }

    if (index >= len) {
        line[0] = '\0';
        return line;
    }

    if (index == 0) {
        return line;
    }

    memmove(line, &line[index], len - index);
    line[len - index] = '\0';

    return line;
}

Variant yaf_config_ini_format(const Object& object, 
        const Variant& config)
{
    auto ptr_readonly = object->o_realProp(YAF_CONFIG_PROPERT_NAME_READONLY, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
    Object tmp_null = null_object;
    return yaf_config_ini_instance(tmp_null, config, *ptr_readonly);
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
        if (vec.size() == 0) {
            arr.set(String(section), String(value));
            return 0;
        } else {
            Variant& cur_var = arr.lvalAt(String(section));
            Array& cur = cur_var.toArrRef();
            build_array(cur, vec, value);
        }
    } else {
        if (vec.size() == 0) {
            arr.set(String(section), String(value));
        } else {
            arr.set(String(section), Array::Create());

            Variant& cur_var = arr.lvalAt(String(section));
            Array& cur = cur_var.toArrRef();
            return build_array(cur, vec, value);
        }
    }

    return 0;
}

static void format_ini_field(Variant& key, Variant& value, Array& dest)
{
    std::string str_key;
    std::string str_value;
    if (key.isString()) {
        str_key = key.toString().toCppString(); 
    }

    if (value.isString()) {
        str_value = value.toString().toCppString();
    }

    if (str_key.length() == 0) {
        dest.set(key, value);
        return;
    }

    std::vector<std::string> vec = split((char*)str_key.c_str(), '.');
    if (vec.size() == 1) {
        dest.set(key, value);
        return;
    } else {
        build_array(dest, vec, str_value);
    }
}

static void format_ini_array(Array& config, Array& dest)
{
    ArrayIter iter = config.begin();

    while (!iter.end()) {
        Variant key = iter.first();
        Variant value = iter.second();

        if (value.isArray()) {
            dest.set(key, Array::Create());
            Variant& cur_var = dest.lvalAt(key.toString());
            Array& cur_arr = cur_var.toArrRef();
            format_ini_array(value.toArrRef(), cur_arr);
        } else {
            format_ini_field(key, value, dest);
        }

        iter.next();
    }
}

static void merge_array(Array& dest, const Array& src)
{
    ArrayIter iter = src.begin();
    while (!iter.end()) {
        Variant key = iter.first();
        Variant value = iter.second();

        if (!dest.exists(key)) {
            dest.set(key, value);
        } else {
            Variant dest_value = dest[key];
            if (dest_value.isArray() && value.isArray()) {
                merge_array(dest_value.toArrRef(), value.toCArrRef());
                dest.set(key, dest_value);
            } 
        }
        iter.next();
    }
}

static Array filter_ini_array(Array& config)
{
    Array arr = Array::Create();
    ArrayIter iter = config.begin();
    char str_key[256];
    char buf[256];

    while (!iter.end()) {
        Variant key = iter.first();
        Variant value = iter.second();

        snprintf(str_key, sizeof(str_key), "%s", key.toString().c_str());

        std::vector<std::string> vec = split(str_key, ':');
        if (vec.size() == 1) {
            arr.set(key, value);
        } else {
            Array tmp = Array::Create();
            for (int i = 1; i < vec.size(); i++) {
                snprintf(buf, sizeof(buf), "%s", vec[1].c_str());
                trim(buf);

                Variant base;
                if(config.exists(String(buf))) {
                    base = config[String(buf)];
                } else if (arr.exists(String(buf))){
                    base = arr[String(buf)];
                }else {
                    continue;
                }

                //合并父节点的配置
                if (base.isArray()) {
                    ArrayIter iter = base.toArray().begin();
                    while (!iter.end()) {
                        tmp.set(iter.first(), iter.second());
                        iter.next();
                    }
                } 
            }

            //合并自己的配置
            if (!value.isArray()) {
                value = Array::Create();
            }

            merge_array(value.toArrRef(), tmp);
            arr.set(String(vec[0]), value);
        }

        iter.next();
    }

    return arr;
}


static int yaf_unserialize_array(YafCacheConfig& cache_config, Array& config)
{
    int type = 0;
    int size = 0;

    HHVM_YAF_UNPACK_INT(cache_config, size);
    for (int i = 0; i < size; i++) {
        HHVM_YAF_UNPACK_TYPE(cache_config, type);
        if (type != HHVM_YAF_ITEM_TYPE_ENTRY) {
            return HHVM_YAF_FAILED;
        }

        HHVM_YAF_UNPACK_TYPE(cache_config, type);
        if (type != HHVM_YAF_ITEM_TYPE_STR) {
            return HHVM_YAF_FAILED;
        }

        char key[8192];
        HHVM_YAF_UNPACK_STR(cache_config, key, sizeof(key));

        HHVM_YAF_UNPACK_TYPE(cache_config, type);
        if (type == HHVM_YAF_ITEM_TYPE_ARRAY) {
            Array value = Array::Create();
            int ret = yaf_unserialize_array(cache_config, value);
            if (ret != HHVM_YAF_SUCCESS) {
                return ret;
            }

            config.set(String(key), value);
        } else if(type == HHVM_YAF_ITEM_TYPE_STR) {
            char value[8192];
            HHVM_YAF_UNPACK_STR(cache_config, value, sizeof(value));

            config.set(String(key), String(value));
        } else {
            raise_warning("unknown item type:%d", type);
            return HHVM_YAF_FAILED;
        }
    }

    return HHVM_YAF_SUCCESS;
}

static int yaf_serialize_array(YafCacheConfig& cache_config, const Array& config)
{
    HHVM_YAF_PACK_TYPE(cache_config, HHVM_YAF_ITEM_TYPE_ARRAY);
    HHVM_YAF_PACK_INT(cache_config, config.size());

    ArrayIter iter = config.begin();
    while (!iter.end()) {
        Variant first = iter.first();
        Variant second = iter.second();

        HHVM_YAF_PACK_TYPE(cache_config, HHVM_YAF_ITEM_TYPE_ENTRY);
        HHVM_YAF_PACK_TYPE(cache_config, HHVM_YAF_ITEM_TYPE_STR);
        HHVM_YAF_PACK_STR(cache_config, first.toString().c_str());

        if (second.isArray()) {
            yaf_serialize_array(cache_config, second.toArray());
        } else {
            HHVM_YAF_PACK_TYPE(cache_config, HHVM_YAF_ITEM_TYPE_STR);
            HHVM_YAF_PACK_STR(cache_config, second.toString().c_str());
        }

        iter.next();
    }

    return HHVM_YAF_SUCCESS;
}


static Variant yaf_config_ini_unserialize(const String& filename, const Variant& section)
{
    String str;
    if (section.isString()) {
        str = section.toString();
    }

    char key[8192];
    snprintf(key, sizeof(key), "%s#%s", filename.c_str(), str.c_str());

    YafCacheMap::iterator iter = g_yaf_local_data.get()->cache_config_map.find(std::string(key));
    if (iter == g_yaf_local_data.get()->cache_config_map.end()) {
        return init_null_variant;
    }

    YafCacheConfig config_data = iter->second;
    struct stat buf;
    int ret = stat(filename.c_str(), &buf);
    if (ret != 0) {
        return init_null_variant;
    }

    if (buf.st_mtime > config_data.modify_time) {
        free(config_data.data);
        g_yaf_local_data.get()->cache_config_map.erase(iter);
        return init_null_variant;
    }

    Array cache_config = Array::Create();

    int type = 0;
    HHVM_YAF_UNPACK_TYPE(config_data, type); 
    if (type != HHVM_YAF_ITEM_TYPE_ARRAY) {
        //清除序列化的缓存配置
        free(config_data.data);
        g_yaf_local_data.get()->cache_config_map.erase(iter);
        return HHVM_YAF_FAILED;
    }

    ret = yaf_unserialize_array(config_data, cache_config);
    if (ret != HHVM_YAF_SUCCESS) {
        //清除序列化的缓存配置
        free(config_data.data);
        g_yaf_local_data.get()->cache_config_map.erase(iter);
        return init_null_variant;
    }

    return cache_config;
}


static int yaf_config_ini_serialize(const char* filename,  Variant var_configs, 
        const char* section)
{
    if (!var_configs.isArray()) {
        return HHVM_YAF_FAILED;
    }

    String str(section);
    char key[8192];
    snprintf(key, sizeof(key), "%s#%s", filename, str.c_str());

    struct stat buf;
    int ret = stat(filename, &buf);
    if (ret != 0) {
        return HHVM_YAF_FAILED;
    }

    YafCacheConfig cache_config;
    cache_config.data_size = 16 * 1024;
    cache_config.data = (char*) malloc(cache_config.data_size * sizeof(char));
    cache_config.write_pos = 0;
    cache_config.read_pos = 0;
    cache_config.modify_time = buf.st_mtime;

    Array configs = var_configs.toArray();
    ret = yaf_serialize_array(cache_config, configs);
    if (ret != HHVM_YAF_SUCCESS) {
        return ret;
    }

    g_yaf_local_data.get()->cache_config_map[std::string(key)] = cache_config;
    return HHVM_YAF_SUCCESS;
}

static  int parse_ini_file_ex(const char* filename, Object* object)
{
    if (filename == NULL || object == NULL) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                "invald file:%s object:%p", 
                filename, object);
        return HHVM_YAF_FAILED;
    }

    auto ptr_config = (*object)->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");

    String func("parse_ini_file");
    Array args = Array::Create();
    args.append(String(filename));
    args.append(Variant(true));

    Variant ret = vm_call_user_func(func, args);
    if (!ret.isArray()) {
        return HHVM_YAF_FAILED;
    }

    Array& config = ret.toArrRef();
    Array dest = Array::Create();
    
    //把 a.b.c=d的格式转换成
    //array(
    //    "a"=>array(
    //      "b" => "c"
    //    )
    //)
    //
    format_ini_array(config, dest);
    //实现yaf配置继承特性
    Array conf = filter_ini_array(dest);
    if (g_yaf_local_data.get()->ini_wanted_section.length()) {
        String tmp_section(g_yaf_local_data.get()->ini_wanted_section);
        if (conf.exists(tmp_section)) {
            *ptr_config = conf[tmp_section];
        } else {
             yaf_trigger_error(YAF_ERR_TYPE_ERROR, "There is no section '%s' in '%s'",
                     tmp_section.c_str(), filename);
        }
    } else {
        *ptr_config = conf;
    }

    bool cache = g_yaf_local_data.get()->cache_config;
    if (cache) {
        yaf_config_ini_serialize(filename, *ptr_config, 
                g_yaf_local_data.get()->ini_wanted_section.c_str());
    }

    return HHVM_YAF_SUCCESS;
}

Variant yaf_config_ini_instance(const Object& object, 
        const Variant& filename, const Variant& section)
{
    Object o;
    if (object.isNull()) {
        Array arr = Array::Create();
        arr.append(filename);
        arr.append(section);

        o = createObject("Yaf_Config_Ini", arr) ;
    } else {
        o = object;
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

    bool cache = g_yaf_local_data.get()->cache_config;
    if (cache) {
        //use cache configs
        Variant configs = yaf_config_ini_unserialize(filename.toString(), section);
        if (configs.isArray()) {
            *ptr_config = configs;
            return o;
        }
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
                "Argument is not a valid ini file '%s'", 
                str_filename.c_str(), errno);
        return init_null_variant;
    }

    g_yaf_local_data.get()->ini_wanted_section.clear();
    if (section.isString()) {
        g_yaf_local_data.get()->ini_wanted_section = section.toString().toCppString();
    }

    ret = parse_ini_file_ex(str_filename.c_str(), &o);
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

    (void)yaf_config_ini_instance(this_, filename, section);
}

static Variant HHVM_METHOD(Yaf_Config_Ini, get, const Variant& name)
{
    if (name.isNull() || 
            (name.isString() && name.toString().length() == 0)) {
        return this_;
    }

    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");

    if (!ptr_config->isArray()) {
        *ptr_config = Array::Create();
        return init_null_variant;
    } 

    Array& arr = ptr_config->toArrRef();
    if (!arr.exists(name.toString())) {
        return init_null_variant;
    }

    Variant value = arr[name.toString()];
    if (value.isArray()) {
        Variant instance = yaf_config_ini_format(this_, value);
        return instance;
    }

    return value;
}

static bool HHVM_METHOD(Yaf_Config_Ini, __isset, const Variant& name)
{
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

    return true;
}

static Variant HHVM_METHOD(Yaf_Config_Ini, set, const Variant& name, const Variant& value)
{
    return false;
}

static Variant HHVM_METHOD(Yaf_Config_Ini, count) 
{
    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
    if (!ptr_config->isArray()) {
        return 0;
    }

    Array& arr = ptr_config->toArrRef();
    return arr.size();
}

static Variant HHVM_METHOD(Yaf_Config_Ini, rewind) 
{
    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
    if (!ptr_config->isArray()) {
        return false;
    }

    Array& arr = ptr_config->toArrRef();
    auto ptr_cursor = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_CURSOR, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");

    *ptr_cursor = Variant(NEWOBJ(yaf_config_cursor)(arr.begin()));
    return true;
}

static Variant HHVM_METHOD(Yaf_Config_Ini, current) 
{
    auto ptr_cursor = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_CURSOR, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");

    auto res = ptr_cursor->toResource();
    ArrayIter& cursor = res.getTyped<yaf_config_cursor>()->cursor();
    Variant value =  cursor.second();
    if (!value.isArray()) {
        return value;
    }

    return yaf_config_ini_format(this_, value);
}

static Variant HHVM_METHOD(Yaf_Config_Ini, next) 
{
    auto ptr_cursor = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_CURSOR, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
    if (ptr_cursor->isNull()) {
        return false;
    }
    
    auto res = ptr_cursor->toResource();
    ArrayIter& cursor = res.getTyped<yaf_config_cursor>()->cursor();
    cursor.next();
    return true;
}

static Variant HHVM_METHOD(Yaf_Config_Ini, valid) 
{
    auto ptr_cursor = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_CURSOR, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
    if (ptr_cursor->isNull()) {
        return false;
    }
 
    auto res = ptr_cursor->toResource();
    ArrayIter& cursor = res.getTyped<yaf_config_cursor>()->cursor();
    return !cursor.end();
}

static Variant HHVM_METHOD(Yaf_Config_Ini, key)
{
    auto ptr_cursor = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_CURSOR, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");

    if (ptr_cursor->isNull()) {
        auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
                ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
        if (!ptr_config->isArray()) {
            return false;
        }

        Array& arr = ptr_config->toArrRef();
        //auto ptr_cursor = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_CURSOR, 
        //        ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
        *ptr_cursor = Variant(NEWOBJ(yaf_config_cursor)(arr.begin()));
    } 

    auto res = ptr_cursor->toResource();
    ArrayIter& cursor = res.getTyped<yaf_config_cursor>()->cursor();

    if (cursor.first().isString()) {
        return cursor.first().toString();
    } else if (cursor.first().isInteger()) {
        return cursor.first().toInt64();
    }

    return cursor.first();
}

static Variant HHVM_METHOD(Yaf_Config_Ini, toArray)
{
    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
    if (!ptr_config->isArray()) {
        *ptr_config = Array::Create();
    }

    return ptr_config->toArray();
}

static Variant HHVM_METHOD(Yaf_Config_Ini, readonly)
{
    auto ptr_config = this_->o_realProp(YAF_CONFIG_PROPERT_NAME_READONLY, 
            ObjectData::RealPropUnchecked, "Yaf_Config_Ini");
    return ptr_config->toBoolean();
}

static Variant HHVM_METHOD(Yaf_Config_Ini, offsetUnset, const Variant& name)
{
    return false;
}

static Variant HHVM_METHOD(Yaf_Config_Ini, offsetGet, const Variant& name)
{
    return false;
}

static Variant HHVM_METHOD(Yaf_Config_Ini, offsetExists, const Variant& name)
{
    return false;
}
 
static Variant HHVM_METHOD(Yaf_Config_Ini, offsetSet, const Variant& name, const String& value)
{
    return false;
}

static Variant HHVM_METHOD(Yaf_Config_Ini, __get, const Variant& name)
{
    if (name.isNull() || 
            (name.isString() && name.toString().length() == 0)) {
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
        return init_null_variant;
    }

    Variant value = arr[name.toString()];
    if (value.isArray()) {
        Variant instance = yaf_config_ini_format(this_, value);
        return instance;
    }

    return value;
}

static Variant HHVM_METHOD(Yaf_Config_Ini, __set, const Variant& name, const Variant& value)
{
    return false;
}
 
void YafExtension::_initYafConfigIniClass()
{
    HHVM_ME(Yaf_Config_Ini, __construct);
    HHVM_ME(Yaf_Config_Ini, __isset);
    HHVM_ME(Yaf_Config_Ini, get);
    HHVM_ME(Yaf_Config_Ini, set);
    HHVM_ME(Yaf_Config_Ini, count);

    HHVM_ME(Yaf_Config_Ini, rewind);
    HHVM_ME(Yaf_Config_Ini, current);
    HHVM_ME(Yaf_Config_Ini, next);
    HHVM_ME(Yaf_Config_Ini, valid);
    HHVM_ME(Yaf_Config_Ini, key);

    HHVM_ME(Yaf_Config_Ini, toArray);
    HHVM_ME(Yaf_Config_Ini, readonly);
    HHVM_ME(Yaf_Config_Ini, offsetUnset);

    HHVM_ME(Yaf_Config_Ini, __set);
    HHVM_ME(Yaf_Config_Ini, __get);
    HHVM_ME(Yaf_Config_Ini, offsetGet);
    HHVM_ME(Yaf_Config_Ini, offsetExists);
    HHVM_ME(Yaf_Config_Ini, offsetSet);

}

}
