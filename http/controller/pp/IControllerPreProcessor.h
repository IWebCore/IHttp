#pragma once

#include "core/util/IPreProcessorUtil.h"

#define $AsController(path)                                                                       \
    Q_CLASSINFO(PP_STRING(IHttpControllerMapping$), #path)                                        \
public:                                                                                           \
    virtual IGadgetUnit::StaticMetacallFunction staticMetaCallFunction() const final{             \
        return qt_static_metacall;                                                                \
    }                                                                                             \
private:

#define PP_CONTROLLER_PROP(name, type, index)    \
    PP_STRING(IHttpControllerCallableMapping$$$ ## name ## $$$ ## type ## $$$ ## index)

#ifndef Q_MOC_RUN
    #define $Path(name)       name
    #define $Query(name)      name
    #define $Header(name)     name
    #define $Cookie(name)     name
    #define $Session(name)    name
    #define $Body(name)       name
    #define $Form(name)       name

    #define $Json_1(name)       name
    #define $Json_2(name1, name2)  name1
    #define $Json_3(name1, name2, name3)  name1
    #define $Json_4(name1, name2, name3, name4)  name1
    #define $Json_5(name1, name2, name3, name4, name5)  name1
    #define $Json_6(name1, name2, name3, name4, name5, name6)  name1
    #define $Json_7(name1, name2, name3, name4, name5, name6, name7)  name1
    #define $Json_8(name1, name2, name3, name4, name5, name6, name7, name8)  name1
    #define $Json_9(name1, name2, name3, name4, name5, name6, name7, name8, name9)  name1

    #define $OptQuery_1(name)  name
    #define $OptQuery_2(name, arg1)  name

    #define $OptHeader_1(name)  name
    #define $OptHeader_2(name, arg1) name

    #define $OptCookie_1(name)  name
    #define $OptCookie_2(name, arg1) name

    #define $OptSession_1(name) name
    #define $OptSession_2(name, arg1)  name

    #define $OptForm_1(name)  name
    #define $OptForm_2(name, arg1)  name
#else
    #define $Path(name)       name##_$PATH
    #define $Query(name)      name##_$QUERY
    #define $Header(name)     name##_$HEADER
    #define $Cookie(name)     name##_$COOKIE
    #define $Session(name)    name##_$SESSION
    #define $Body(name)       name##_$BODY
    #define $Form(name)       name##_$FORM

    #define $Json_1(name)  \
        name##_$JSON
    #define $Json_2(name1, name2)  \
        name1##_$JSON##_$##name2
        name1##_$JSON##_$##name2##_$##name3
    #define $Json_4(name1, name2, name3, name4)  \
        name1##_$JSON##_$##name2##_$##name3##_$##name4
    #define $Json_5(name1, name2, name3, name4, name5)  \
        name1##_$JSON##_$##name2##_$##name3##_$##name4##_$##name5
        name1##_$JSON##_$##name2##_$##name3##_$##name4##_$##name5##_$##name6
    #define $Json_7(name1, name2, name3, name4, name5, name6, name7)  \
        name1##_$JSON##_$##name2##_$##name3##_$##name4##_$##name5##_$##name6##_$##name7
    #define $Json_8(name1, name2, name3, name4, name5, name6, name7, name8)  \
        name1##_$JSON##_$##name2##_$##name3##_$##name4##_$##name5##_$##name6##_$##name7##_$##name8
    #define $Json_9(name1, name2, name3, name4, name5, name6, name7, name8, name9)  \
        name1##_$JSON##_$##name2##_$##name3##_$##name4##_$##name5##_$##name6##_$##name7##_$##name8##_$##name9

    #define $OptQuery_1(name)  name##_$QUERY_$OPTIONAL
    #define $OptQuery_2(name, arg1)  name##_$QUERY_$OPTIONAL##_$##arg1

    #define $OptHeader_1(name)   name##_$HEADER_$OPTIONAL
    #define $OptHeader_2(name, arg1)   name##_$HEADER_$OPTIONAL##_$##arg1

    #define $OptCookie_1(name)   name##_$COOKIE_$OPTIONAL
    #define $OptCookie_2(name, arg1)   name##_$COOKIE_$OPTIONAL##_$##arg1

    #define $OptSession_1(name)    name##_$SESSION_$OPTIONAL
    #define $OptSession_2(name, arg1)  name##_$SESSION_$OPTIONAL##_$##arg1

    #define $OptForm_1(name)   name##_$FORM_$OPTIONAL
    #define $OptForm_2(name, arg1)  name##_$FORM_$OPTIONAL##_$##arg1
#endif

#define $Json_(N) $Json_##N
#define $Json_EVAL(N) $Json_(N)
#define $Json(...) PP_EXPAND( $Json_EVAL(PP_EXPAND( PP_NARG(__VA_ARGS__) ))(__VA_ARGS__) )

#define $OptQuery_(N) $OptQuery_##N
#define $OptQuery_EVAL(N) $OptQuery_(N)
#define $OptQuery(...) PP_EXPAND( $OptQuery_EVAL(PP_EXPAND( PP_NARG(__VA_ARGS__) ))(__VA_ARGS__) )

#define $OptHeader_(N) $OptHeader_##N
#define $OptHeader_EVAL(N) $OptHeader_(N)
#define $OptHeader(...) PP_EXPAND( $OptHeader_EVAL(PP_EXPAND( PP_NARG(__VA_ARGS__) ))(__VA_ARGS__) )


#define $OptCookie_(N) $OptCookie_##N
#define $OptCookie_EVAL(N) $OptCookie_(N)
#define $OptCookie(...) PP_EXPAND( $OptCookie_EVAL(PP_EXPAND( PP_NARG(__VA_ARGS__) ))(__VA_ARGS__) )

#define $OptSession_(N) $OptSession_##N
#define $OptSession_EVAL(N) $OptSession_(N)
#define $OptSession(...) PP_EXPAND( $OptSession_EVAL(PP_EXPAND( PP_NARG(__VA_ARGS__) ))(__VA_ARGS__) )

#define $OptForm_(N) $OptForm_##N
#define $OptForm_EVAL(N) $OptForm_(N)
#define $OptForm(...) PP_EXPAND( $OptForm_EVAL(PP_EXPAND( PP_NARG(__VA_ARGS__) ))(__VA_ARGS__) )

