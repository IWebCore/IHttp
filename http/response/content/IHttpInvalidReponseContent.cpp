#include "IHttpInvalidReponseContent.h"
#include "http/detail/IHttpResponseRaw.h"

$PackageWebCoreBegin

IHttpInvalidReponseContent::IHttpInvalidReponseContent(const IHttpInvalidWare &ware)
    :IHttpResponseContent(ware.description)
{
    if(ware.m_processor){
        m_processor = [=](const IHttpResponseContent&content, IHttpResponseRaw& raw){
            ware.m_processor(ware, raw);
        };
    }
}

$PackageWebCoreEnd
