#pragma once

#include "core/config/IConfigPreProcessor.h"

#define $SetHttpAssetsEnabled( boolValue )  \
    PP_PROFILE_CONFIG(SetHttpAssetsEnabled, /http/assets/enabled, boolValue)

#define $SetHttpAssetsPath( path_str )  \
    PP_PROFILE_CONFIG(SetHttpAssetsPath, /http/assets/path, path_str)
