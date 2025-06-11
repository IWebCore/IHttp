#pragma once

#include "core/util/IPreProcessorUtil.h"

#define $AsController(path)                                                                       \
    Q_CLASSINFO(PP_STRING(IHttpControllerMapping$), #path)                                        \
public:                                                                                           \
    virtual IGadgetUnit::StaticMetacallFunction staticMetaCallFunction() const final{             \
        return qt_static_metacall;                                                                \
    }                                                                                             \
private:

#define PP_CONTROLLER_PROP(funName, type, index)    \
    PP_STRING(IHttpControllerCallableMapping$$$ ## funName ## $$$ ## type ## $$$ ## index)    

#ifdef Q_MOC_RUN
    #define $Path(name)       name##_$PATH
    #define $Query(name)      name##_$QUERY
    #define $Header(name)     name##_$HEADER
    #define $Cookie(name)     name##_$COOKIE
    #define $Session(name)    name##_$SESSION
    #define $Body(name)       name##_$BODY
    #define $Form(name)       name##_$FORM
    #define $Json(name)       name##_$JSON
    #define $Optional(name)   name##_$OPTIONAL

#else
    #define $Path(name)       name
    #define $Query(name)      name
    #define $Header(name)     name
    #define $Cookie(name)     name
    #define $Session(name)    name
    #define $Body(name)       name
    #define $Form(name)       name
    #define $Json(name)       name
    #define $Optional(name)   name

#endif

