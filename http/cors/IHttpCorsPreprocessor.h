#pragma once

#include "http/controller/pp/IControllerPreProcessor.h"

#define $CrossOrigin_1(funName)  \
    Q_CLASSINFO( PP_CONTROLLER_CALLABLE_PROPERTY(funName, cors, 1), "*")

#define $CrossOrigin_2(funName, url1)  \
    Q_CLASSINFO( PP_CONTROLLER_CALLABLE_PROPERTY(funName, cors, 1), #url1)

#define $CrossOrigin_3(funName, url1, url2)  \
    $CrossOrigin(funName, url1) \
    Q_CLASSINFO( PP_CONTROLLER_CALLABLE_PROPERTY(funName, cors, 2), #url2)

#define $CrossOrigin_4(funName, url1, url2, url3)  \
    $CrossOrigin(funName, url1, url2) \
    Q_CLASSINFO( PP_CONTROLLER_CALLABLE_PROPERTY(funName, cors, 3), #url3)

#define $CrossOrigin_5(funName, url1, url2, url3, url4)  \
    $CrossOrigin(funName, url1, url2, url3) \
    Q_CLASSINFO( PP_CONTROLLER_CALLABLE_PROPERTY(funName, cors, 4), #url4)

#define $CrossOrigin_6(funName, url1, url2, url3, url4, url5)  \
    $CrossOrigin(funName, url1, url2, url3, url4) \
    Q_CLASSINFO( PP_CONTROLLER_CALLABLE_PROPERTY(funName, cors, 5), #url5)

#define $CrossOrigin_7(funName, url1, url2, url3, url4, url5, url6)  \
    $CrossOrigin(funName, url1, url2, url3, url4, url5) \
    Q_CLASSINFO( PP_CONTROLLER_CALLABLE_PROPERTY(funName, cors, 6), #url6)

#define $CrossOrigin_8(funName, url1, url2, url3, url4, url5, url6, url7)  \
    $CrossOrigin(funName, url1, url2, url3, url4, url5, url6) \
    Q_CLASSINFO( PP_CONTROLLER_CALLABLE_PROPERTY(funName, cors, 7), #url7)

#define $CrossOrigin_9(funName, url1, url2, url3, url4, url5, url6, url7, url8)  \
    $CrossOrigin(funName, url1, url2, url3, url4, url5, url6, url7) \
    Q_CLASSINFO( PP_CONTROLLER_CALLABLE_PROPERTY(funName, cors, 8), #url8)

#define $CrossOrigin_(N) $CrossOrigin_##N
#define $CrossOrigin_EVAL(N) $CrossOrigin_(N)
#define $CrossOrigin(...) PP_EXPAND( $CrossOrigin_EVAL(PP_EXPAND( PP_NARG(__VA_ARGS__) ))(__VA_ARGS__) )
