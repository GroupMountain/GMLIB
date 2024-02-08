#include "Global.h"
#include <GMLIB/Mod/CustomPacks.h>

namespace GMLIB::Mod {

std::vector<std::string> mAllResourcePath;
std::vector<std::string> mPackListCache;
bool                     mCustomPackEnabled = false;

void addResourcePackPath(ResourcePackRepository* repo, PackType type) {
    auto  CompositePack     = ll::memory::dAccess<CompositePackSource*>(repo, 96);
    auto& PackSourceFactory = repo->getPackSourceFactory();
    for (auto& path : mAllResourcePath) {
        auto& DirectoryPackSource =
            PackSourceFactory.createDirectoryPackSource(Core::Path(path), type, PackOrigin::Test, 0);
        CompositePack->addPackSource(*(PackSource*)&DirectoryPackSource);
    }
    repo->refreshPacks();
}

LL_TYPE_INSTANCE_HOOK(
    PacksBuildEvent,
    ll::memory::HookPriority::Normal,
    ResourcePack,
    "??0ResourcePack@@QEAA@AEAVPack@@@Z",
    ResourcePack*,
    Pack* a1
) {
    auto pack     = origin(a1);
    auto manifest = &pack->getManifest();
    if (manifest && manifest->getPackOrigin() == PackOrigin::Test) {
        mPackListCache.push_back(a1->getManifest().getIdentity().asString());
    }
    return pack;
}

LL_STATIC_HOOK(
    PacksLoadEvent,
    ll::memory::HookPriority::Normal,
    "?deserialize@ResourcePackStack@@SA?AV?$unique_ptr@VResourcePackStack@@U?$default_delete@VResourcePackStack@@@"
    "std@@@std@@AEAV?$basic_istream@DU?$char_traits@D@std@@@3@AEBV?$not_null@V?$NonOwnerPointer@$$"
    "CBVIResourcePackRepository@@@Bedrock@@@gsl@@@Z",
    std::unique_ptr<ResourcePackStack>,
    std::istream&                                                                 a1,
    gsl::not_null<Bedrock::NonOwnerPointer<IResourcePackRepository const>> const& a2
) {
    auto stack = origin(a1, a2);
    for (auto id : mPackListCache) {
        auto  pack = ll::service::getResourcePackRepository()->getResourcePackForPackId(PackIdVersion::fromString(id));
        auto& SettingsFactory = ll::service::getResourcePackRepository()->getPackSettingsFactory();
        auto  settings        = SettingsFactory.getPackSettings(pack->getManifest());
        char  gsl[16];
        auto gslPath = LL_SYMBOL_CALL("??$?0PEAVResourcePack@@X@?$not_null@V?$NonOwnerPointer@VResourcePack@@@Bedrock@@@gsl@@"
                               "QEAA@AEBV?$not_null@PEAVResourcePack@@@1@@Z",
                               void*, char*, ResourcePack**)(gsl, &pack);
        char size[480];
        auto Instance =
            LL_SYMBOL_CALL("??0PackInstance@@QEAA@V?$not_null@V?$NonOwnerPointer@VResourcePack@@@Bedrock@@@gsl@@H_"
                    "NPEAVPackSettings@@@Z",
                    PackInstance*, char*, void*, int, bool, PackSettings*)(size, gslPath, 0, false, settings);

        stack->add(*Instance, a2, 0);
    }
    mPackListCache.clear();
    return stack;
}

LL_TYPE_INSTANCE_HOOK(
    ResourcePackRepositoryInitEvent,
    ll::memory::HookPriority::Normal,
    ResourcePackRepository,
    "?_initialize@ResourcePackRepository@@AEAAXXZ",
    void
) {
    addResourcePackPath(this, PackType::Behavior);
    addResourcePackPath(this, PackType::Resources);
    return origin();
}

void CustomPacks::addCustomPackPath(std::string path) {
    if (!mCustomPackEnabled) {
        ll::memory::HookRegistrar<PacksBuildEvent>().hook();
        ll::memory::HookRegistrar<ResourcePackRepositoryInitEvent>().hook();
        ll::memory::HookRegistrar<PacksLoadEvent>().hook();
        mCustomPackEnabled = true;
    }
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directories(path);
    }
    mAllResourcePath.push_back(path);
}

} // namespace GMLIB::Mod