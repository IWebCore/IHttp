#include "IHttpInvalidReponseContent.h"
#include "http/IHttpConstant.h"
#include "http/biscuits/IHttpMime.h"
#include "http/detail/IHttpResponseRaw.h"

$PackageWebCoreBegin

IHttpInvalidReponseContent::IHttpInvalidReponseContent(const IHttpInvalidWare &ware)
    :IHttpResponseContent(ware.description)
{
    setAttribute(&TYPE_NAME, &IHttpInvalidReponseContent::Type);

    if(ware.m_processor){
        m_processor = [=](const IHttpResponseContent&, IHttpResponseRaw& raw){
            ware.m_processor(ware, raw);
        };
    }
}

$PackageWebCoreEnd
