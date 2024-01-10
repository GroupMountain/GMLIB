#pragma once
#include "GMLIB/GMLIB.h"

namespace GMLIB::Mod::Recipe {

class CustomFurnaceRecipe {
public:

    virtual ItemInstance getInput() = 0;

    virtual ItemInstance getResult() = 0;

    virtual std::vector<HashedString> getCraftingTags() = 0;
};
} // namespace GMLIB::Mod::Recipe