#include "Global.h"
#include <GMLIB/Mod/Addons/VanillaFix.h>
#include <mc/locale/I18n.h>
#include <mc/resources/ResourcePackManager.h>
#include <mc/server/ServerInstance.h>
#include <mc/server/common/PropertiesSettings.h>

namespace GMLIB::Mod {

std::thread I18n_thread;
bool        isResourcesLoaded = false;
std::mutex  mtx;

LL_STATIC_HOOK(
    I18nFix1,
    HookPriority::Normal,
    "?chooseLanguage@I18n@@SAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
    void,
    std::string const& code
) {
    ll::memory::HookRegistrar<I18nFix1>().unhook();
}

LL_TYPE_INSTANCE_HOOK(
    I18nFix2,
    HookPriority::Normal,
    I18n,
    "?loadAllLanguages@I18n@@SAXAEAVResourcePackManager@@@Z",
    void,
    class ResourcePackManager& resourcePackManager
) {
    I18n_thread = std::thread([&]() {
        while (!isResourcesLoaded) {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
        mtx.lock();
        origin(resourcePackManager);
        auto lang = ll::service::getPropertiesSettings()->getLanguage();
        I18n::chooseLanguage(lang);
        mtx.unlock();
    });
}

LL_TYPE_INSTANCE_HOOK(
    I18nFix3,
    HookPriority::Normal,
    ServerInstance,
    "?setUnrecoverableErrorCallback@ServerInstance@@QEAAXV?$function@$$A6AXPEBD0@Z@std@@@Z",
    void,
    std::function<void(char const*, char const*)> func
) {
    origin(func);
    isResourcesLoaded = true;
    I18n_thread.join();
}

struct I18n_Impl {
    ll::memory::HookRegistrar<I18nFix1, I18nFix2, I18nFix3> r;
};

std::unique_ptr<I18n_Impl> impl;

void VanillaFix::setFixI18nEnabled() {
    if (!impl) {
        impl = std::make_unique<I18n_Impl>();
    }
}

} // namespace GMLIB::Mod