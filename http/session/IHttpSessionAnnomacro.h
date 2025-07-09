#pragma once

#include "core/util/IPreProcessorUtil.h"
#include "core/config/IConfigPreProcessor.h"

#define $SetHttpSessionEnabled(boolValue)   \
    PP_PROFILE_CONFIG(SetHttpSessionEnabledKlass, /http/session/enabled, boolValue)

#define $SetHttpSessionExpiredTime(intValue) \
    PP_PROFILE_CONFIG(SetHttpSessionExpireTimeKlass, /http/session/expiredTime, intValue)

#define $SetHttpSessionClearTime(intValue)  \
    PP_PROFILE_CONFIG(SetHttpSessionClearTimeKlass, /http/session/clearTime, intValue)

#define $SetHttpSessionAutoCreate(boolValue)    \
    PP_PROFILE_CONFIG(SetHttpSessionAutoCreateKlass, "/http/session/autoCreate", boolValue)

#define $SetHttpSessionCookieHttpOnly(boolValue)  \
    PP_PROFILE_CONFIG(SetHttpSessionCookieHttpOnlyKlass, /http/session/cookie/httpOnly, boolValue)

#define $SetHttpSessionCookieSecure(boolValue)    \
    PP_PROFILE_CONFIG(SetHttpSessionCookieSecureKlass, /http/session/cookie/secure, boolValue)

#define $SetHttpSessionCookiePath(StringValue)  \
    PP_PROFILE_CONFIG(SetHttpSessionCookiePathKlass, /http/session/cookie/path, #StringValue)
