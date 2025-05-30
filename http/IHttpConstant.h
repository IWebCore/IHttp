#pragma once

#include "core/util/IPackageUtil.h"

$PackageWebCoreBegin

$IPackageBegin(IHttp)

static inline constexpr IStringView NEW_LINE = "\r\n";
static inline constexpr IStringView NEW_PART = "\r\n\r\n";
static inline constexpr IStringView SPACE = " ";
static inline constexpr IStringView COMMA_SPACE = ": ";
static inline constexpr IStringView SEMICOLON = ";";
static inline constexpr IStringView EQUAL = "=";
static inline constexpr IStringView EMPTY = "";
static inline constexpr IStringView FALSE_STR = "False";
static inline constexpr IStringView TRUE_STR = "True";

static inline constexpr IStringView SESSION_HEADER = "ISessionId";

static constexpr std::size_t HTTP_BASE_MESSAGE_SIZE = 1024*10;

$IPackageEnd(IHttp)

$PackageWebCoreEnd
