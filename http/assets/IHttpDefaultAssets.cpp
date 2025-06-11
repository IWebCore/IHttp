#include "IHttpDefaultAssets.h"
#include "core/config/IProfileImport.h"
#include "http/assets/IHttpAssetsAction.h"

$PackageWebCoreBegin

IHttpDefaultAssets::IHttpDefaultAssets()
{
}

bool IHttpDefaultAssets::isValid() const
{
    static $Bool enabled{"/http/assets/enabled", true};
    if(!enabled.value()){
        return false;
    }

    static $QString path{"/http/assets/path", ""};
    if(path.value().isEmpty()){
        return false;
    }

    return true;
}

void IHttpDefaultAssets::travelPrint() const
{
    static $QString path{"/http/assets/path"};
    qDebug() << "assets mapping file at:" << *path << endl;
}

IHttpActionWare *IHttpDefaultAssets::getAction(IRequest &request) const
{
    static $QString s_path{"/http/assets/path"};

    auto path = request.url();
    auto newPath = s_path.value() + path.toQString();
    if(QFileInfo(newPath).isFile() && QFileInfo(newPath).exists()){
        return new IHttpAssetsAction(newPath);
    }

    // other path do not process
    if(path.empty() || path == "/"){
        newPath = s_path.value() + "/index.html";
        if(QFileInfo(newPath).exists()){
            return new IHttpAssetsAction(newPath);
        }
        newPath = s_path.value() + "/index.htm";
        if(QFileInfo(newPath).exists()){
            return new IHttpAssetsAction(newPath);
        }
        newPath = s_path.value() + "/default.html";
        if(QFileInfo(newPath).exists()){
            return new IHttpAssetsAction(newPath);
        }
        newPath = s_path.value() + "/default.htm";
        if(QFileInfo(newPath).exists()){
            return new IHttpAssetsAction(newPath);
        }
    }

    return nullptr;
}

$PackageWebCoreEnd
