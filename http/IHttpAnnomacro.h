#pragma once

#include "core/config/IConfigPreProcessor.h"

#define $SetHttpIp(value)    \
    PP_PROFILE_CONFIG(SetHttpIpAddress, /http/ip, value)

#define $SetHttpPort(value)   \
    PP_PROFILE_CONFIG(SetHttpIpPort, /http/port, value)

#define $EnableHttpPrintTrace(boolValue) \
    PP_PROFILE_CONFIG(EnableHttpPrintTraceKlass, /http/printTrace, boolValue)

#define $EnableHttpSession(boolValue)   \
    PP_PROFILE_CONFIG(EnableHttpSessionKlass, /http/session/enabled, boolValue)

#define $EnableHttpSessionAutoCreate(boolValue)    \
    PP_PROFILE_CONFIG(EnableHttpSessionAutoCreateKlass, /http/session/autoCreate, boolValue)
