#pragma once
#include "GMLIB/GMLIB.h"
#include <mc/world/item/ItemInstance.h>
#include <mc/world/item/crafting/Recipes.h>


namespace GMLIB::Mod {

class CustomShulkerBoxRecipe {
public:
    virtual std::string getRecipeId() = 0;

    virtual ItemInstance getResult() = 0;

    virtual std::vector<Recipes::Type> getIngredients() = 0;

    virtual std::vector<HashedString> getCraftingTags() { return {"crafting_table"}; }

    virtual std::optional<class RecipeUnlockingRequirement> getRecipeUnlockingRequirement() { return {}; }

    virtual SemVersion getSemVersion() { return SemVersion(1, 20, 60, "", ""); }
};

} // namespace GMLIB::Mod