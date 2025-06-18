#include "IHttpCorsFilter.h"
#include "http/IRequest.h"
#include "http/detail/IHttpRequestImpl.h"
#include "http/controller/IHttpControllerAction.h"

IHttpCorsFilter::IHttpCorsFilter()
{

}

void IHttpCorsFilter::filter(IRequest &req)
{
    auto action = dynamic_cast<IHttpControllerAction*>(req.impl().m_action);
    if(action && action->m_properties && action->m_properties->contains("cors")){
        auto list = action->m_properties->value("cors");
        qDebug() << "this will be fixed latter" << list;
    }

}
