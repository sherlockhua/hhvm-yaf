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

#define YAF_DEFINE_CLASS(CLS) \
   class CLS { \
   public: \
    static Object allocObject() { \
      if (cls == nullptr) { \
        initClass(); \
      } \
      return ObjectData::newInstance(cls); \
    } \
    \
    static Object allocObject(const Variant& arg) { \
      Object ret = allocObject(); \
      TypedValue dummy; \
      g_context->invokeFunc(&dummy, \
                              cls->getCtor(), \
                              make_packed_array(arg), \
                              ret.get()); \
      return ret; \
    } \
    \
   private: \
    static void initClass() { \
      cls = Unit::lookupClass(StringData::Make(#CLS)); \
    } \
    \
    static HPHP::Class* cls; \
  };

//YAF_DEFINE_CLASS(calc);

}


#endif //_EXT_EXAMPLE_H

