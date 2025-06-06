#include "IHttpAssetsActionMapping.h"

IHttpAssetsActionMapping::IHttpAssetsActionMapping()
{
}

void IHttpAssetsActionMapping::registAssetsWare(IHttpAssetsWare *ware)
{
    m_assetsWares.push_back(ware);
}

void IHttpAssetsActionMapping::travelPrint() const
{

}

IHttpActionWare *IHttpAssetsActionMapping::getAction(IRequest &request) const
{
    for(auto ware : m_assetsWares){
        auto action = ware->getAction(request);
        if(action){
            return action;
        }
    }
    return nullptr;
}

double IHttpAssetsActionMapping::$order() const
{
    return 51;
}
