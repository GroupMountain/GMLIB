#pragma once
#include "GMLIB/Macros.h"
#include "mc/deps/core/sem_ver/SemVersion.h"
#include "mc/world/item/ItemInstance.h"

namespace GMLIB::Mod {

class CustomFurnaceRecipe {
public:
    virtual ItemInstance getInput() = 0;

    virtual ItemInstance getResult() = 0;

    virtual std::vector<HashedString> getCraftingTags() = 0;
};

} // namespace GMLIB::Mod