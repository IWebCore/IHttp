#include "IHttpDefaultAssets.h"
#include "core/config/IProfileImport.h"
#include "http/assets/IHttpAssetsAction.h"

$PackageWebCoreBegin

IHttpDefaultAssets::IHttpDefaultAssets()
{
    qDebug() << "run here";
}

bool IHttpDefaultAssets::isValid() const
{
    return true;
//    static $Bool enabled{"/http/defaultAssets/enabled", true};
//    static $QString path{"/http/defaultAssets/path", ""};
//    if(!enabled.value()){
//        return false;
//    }

//    if(path.value().isEmpty()){
//        return false;
//    }

//    return true;
}

IHttpActionWare *IHttpDefaultAssets::getAction(IRequest &) const
{
    return new IHttpAssetsAction("D:/yuekeyuan.pdf");
}

$PackageWebCoreEnd
