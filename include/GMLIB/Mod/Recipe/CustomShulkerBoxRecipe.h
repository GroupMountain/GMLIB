#pragma once
#include "GMLIB/DllExport.h"

namespace GMLIB::Mod::Recipe {

class CustomShulkerBoxRecipe {
public:
    virtual std::string getRecipeId() = 0;

    virtual ItemInstance getResult() = 0;

    virtual std::vector<Recipes::Type> getIngredients() = 0;

    virtual std::vector<HashedString> getCraftingTags() { return {"crafting_table"}; }

    virtual std::optional<class RecipeUnlockingRequirement> getRecipeUnlockingRequirement() { return {}; }

    virtual SemVersion getSemVersion() { return SemVersion(1, 20, 50, "", ""); }
};


} // namespace GMLIB::Mod::Recipe