#pragma once

#include "core/abort/IAbortInterface.h"

$PackageWebCoreBegin

class IHttpAbort : public IAbortInterface<IHttpAbort>
{
    $AsAbort(
        TypeConvertionFailed,
        ArgumentTypeError,
        HttpPathError,
        HttpPathFragmentError,
        HttpCallableError
    )

protected:
    virtual QMap<int, QString> abortDescription() const final
    {
        return {
            {TypeConvertionFailed, "convert data from one type to another type failed"},
            {ArgumentTypeError, "some error occured when processing method argument"},
            {HttpPathError, "error exist when parse controller mapping path(http path)"},
            {HttpPathFragmentError, "error exist when parse controller mapping path fragment(http path fragment)"},
            {HttpCallableError, "error exist in http callable"}
        };
    }
};

$PackageWebCoreEnd
