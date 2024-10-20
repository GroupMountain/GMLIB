#pragma once
#include "Macros.h"
#include "mc/deps/core/sem_ver/SemVersion.h"
#include "mc/world/item/ItemInstance.h"

namespace gmlib::mod::recipe {

class ICustomFurnaceRecipe {
public:
    virtual ItemInstance getInput() = 0;

    virtual ItemInstance getResult() = 0;

    virtual std::vector<HashedString> getCraftingTags() = 0;
};

} // namespace gmlib::mod::recipe