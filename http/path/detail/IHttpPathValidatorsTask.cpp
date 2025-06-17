#include "IHttpPathValidatorsTask.h"
#include "http/IHttpManage.h"
#include "http/path/IHttpPathFragment.h"
#if __has_include(<charconv>)
    #include <charconv>
#endif

$PackageWebCoreBegin

namespace detail {
    static bool isShortValue(IStringView value);
    static bool isUShortValue(IStringView value);
    static bool isIntValue(IStringView value);
    static bool isUIntValue(IStringView value);
    static bool isLongValue(IStringView value);
    static bool isULongValue(IStringView value);
    static bool isLongLongValue(IStringView value);
    static bool isULongLongValue(IStringView value);
    static bool isFloatValue(IStringView value);
    static bool isDoubleValue(IStringView value);
    static bool isQDateValue(IStringView value);
    static bool isQTimeValue(IStringView value);
    static bool isQDateTimeValue(IStringView value);
    static bool isQStringValue(IStringView value);
}

void IHttpPathValidatorsTask::$task()
{
    QMap<QString, QString> validatorRegMap = {
        {"uuid",    "^[0-9a-f]{8}(-[0-9a-f]{4}){3}-[0-9a-f]{12}$"},
        {"base64",  "^[a-zA-Z0-9+/]*={0,2}$"},
    };

    QMap<QString, IHttpPathFragment::Validator> validatorFunMap = {
        {"short", detail::isShortValue},
        {"ushort", detail::isUShortValue},
        {"int", detail::isIntValue},
        {"uint", detail::isUIntValue},
        {"long", detail::isLongValue},
        {"ulong", detail::isULongValue},
        {"longlong", detail::isLongLongValue},
        {"ulonglong", detail::isULongLongValue},
        {"float", detail::isFloatValue},
        {"double", detail::isDoubleValue},

        {"date", detail::isQDateValue},
        {"QDate", detail::isQDateValue},
        {"time", detail::isQTimeValue},
        {"QTime", detail::isQTimeValue},
        {"datetime", detail::isQDateTimeValue},
        {"QDateTime", detail::isQDateTimeValue},

        {"string", detail::isQStringValue},
        {"QString", detail::isQStringValue},
    };

    for(auto key : validatorRegMap.keys()){
        QRegularExpression exp(validatorRegMap[key]);
        IHttpManage::instance().registerPathValidator(key, [=](IStringView value)->bool{
            qDebug() <<QUuid::createUuid().toString();
            return exp.match(value.toQString()).hasMatch();
        });
    }
    for(auto key : validatorFunMap.keys()){
        IHttpManage::instance().registerPathValidator(key, validatorFunMap[key]);
    }
}

#if __has_include(<charconv>)
template<typename T>
bool isFitIntegerValue(IStringView str)
{
    static_assert(std::is_integral_v<T>, "T must be an integral type");

    size_t start = 0;
    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start]))) {
        ++start;
    }
    if (start == str.size()) return false;
    T value;
    auto [ptr, ec] = std::from_chars(
        str.data() + start,
        str.data() + str.size(),
        value
    );

    return (ec == std::errc() && ptr == str.data() + str.size());
}
#else
template<typename T>
bool isFitIntegerValue(std::string_view str) {
    static_assert(std::is_integral<T>::value, "T must be an integral type");

    char* endptr;
    errno = 0;
    if (std::is_signed<T>::value) {
        long long val = std::strtoll(str.data(), &endptr, 10);
        if (endptr != str.data() + str.size()) return false;
        if (errno == ERANGE) return false;
        if (val < std::numeric_limits<T>::min() || val > std::numeric_limits<T>::max()) {
            return false;
        }
        std::ostringstream oss;
        oss << static_cast<T>(val);
        std::string converted_str = oss.str();
        return (str == converted_str);
    }
    else {
        if (str[0] == '-') return false;
        unsigned long long val = std::strtoull(str.data(), &endptr, 10);
        if (endptr != str.data() + str.size()) return false;
        if (errno == ERANGE) return false;
        if (val > static_cast<unsigned long long>(std::numeric_limits<T>::max())) {
            return false;
        }
        std::ostringstream oss;
        oss << static_cast<T>(val);
        std::string converted_str = oss.str();
        return (str == converted_str);
    }
}

#endif
bool detail::isShortValue(IStringView value)
{
    return isFitIntegerValue<short>(value);
}

bool detail::isUShortValue(IStringView value)
{
    return isFitIntegerValue<ushort>(value);
}

bool detail::isIntValue(IStringView value)
{
    return isFitIntegerValue<int>(value);
}

bool detail::isUIntValue(IStringView value)
{
    return isFitIntegerValue<uint>(value);
}

bool detail::isLongValue(IStringView value)
{
    return isFitIntegerValue<long>(value);
}

bool detail::isULongValue(IStringView value)
{
    return isFitIntegerValue<ulong>(value);
}

bool detail::isLongLongValue(IStringView value)
{
    return isFitIntegerValue<long long>(value);
}

bool detail::isULongLongValue(IStringView value)
{
    return isFitIntegerValue<unsigned long long>(value);
}

bool detail::isFloatValue(IStringView str)
{
    if (str.empty()) {
        return false;
    }
    char* end = nullptr;
    std::strtof(str.data(), &end);
    if (*end != '\0') {
        return false;
    }
    return true;
}

bool detail::isDoubleValue(IStringView str)
{
    if (str.empty()) {
        return false;
    }
    char* end = nullptr;
    std::strtod(str.data(), &end);
    if (*end != '\0') {
        return false;
    }
    return true;
}

bool detail::isQDateValue(IStringView value){
    QVariant variant = value.toQString();
    return variant.toDate().isValid();
}

bool detail::isQTimeValue(IStringView value){
    QVariant variant = value.toQString();
    return variant.toTime().isValid();
}

bool detail::isQDateTimeValue(IStringView value){
    QVariant variant = value.toQString();
    return variant.toDateTime().isValid();
}

bool detail::isQStringValue(IStringView value){
    return !value.empty();
}

$PackageWebCoreEnd
