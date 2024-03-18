#include "Global.h"
#include <GMLIB/Mod/CustomItem/CustomItem.h>
#include <mc/world/item/registry/ItemRegistryRef.h>

namespace GMLIB::Mod {

void CustomItem::unregisterItem(std::string id) {
    ll::service::getLevel()->getItemRegistry().unregisterItem(HashedString(id));
}

} // namespace GMLIB::Mod