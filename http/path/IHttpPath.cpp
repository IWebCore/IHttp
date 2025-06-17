#include "IHttpPath.h"
#include "IHttpPathFragment.h"
#include "core/util/ISpawnUtil.h"

$PackageWebCoreBegin

namespace ISpawnUtil
{
    template<>
    IHttpPath construct<IHttpPath, const std::vector<IHttpPathFragment>&>(const std::vector<IHttpPathFragment>& fragments)
    {
        IHttpPath path;

        QStringList args;
        for(const auto& info : fragments){
            args.push_back(info.m_fragment.toQString());
        }

        path.m_path = args.join("/");
        path.m_fragments = fragments;
        path.m_actionNameMap.clear();
        for(int i=0; i< static_cast<int>(path.m_fragments.size()); i++){
            if(!path.m_fragments[i].m_name.isEmpty()){
                path.m_actionNameMap[path.m_fragments[i].m_name] = i;
            }
        }
        return path;
    }
}

$PackageWebCoreEnd
