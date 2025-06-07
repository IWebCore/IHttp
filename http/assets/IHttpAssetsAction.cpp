#include "IHttpAssetsAction.h"
#include "http/IRequest.h"
#include "http/response/IFileResponse.h"
#include "http/response/content/IHttpFileResponseContent.h"
#include "http/IResponse.h"

IHttpAssetsAction::IHttpAssetsAction(const QString &path)
    : m_path (path)
{
    m_isDeleteble = true;
}

void IHttpAssetsAction::invoke(IRequest &request) const
{
    IResponse response (request);
    response.setResponseWare(IFileResponse(m_path));
    request.startWrite();
}
