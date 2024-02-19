#pragma once
#include "GMLIB/GMLIB.h"
#include <mc/world/item/ItemInstance.h>
#include <mc/world/item/crafting/Recipes.h>

namespace GMLIB::Mod {

class CustomShapelessRecipe {
public:
    virtual std::string getRecipeId() = 0;

    virtual ItemInstance getResult() = 0;

    virtual std::vector<Recipes::Type> getIngredients() = 0;

    virtual std::vector<HashedString> getCraftingTags() { return {"crafting_table"}; }

    virtual int getPriority() { return 50; }

    virtual std::function<std::unique_ptr<
        ShapelessRecipe>(std::string, std::vector<class RecipeIngredient> const&, std::vector<class ItemInstance> const&, class HashedString, int, class mce::UUID const*, std::optional<class RecipeUnlockingRequirement>, class SemVersion const&)>
    getConstructor() {
        return nullptr;
    }

    virtual std::optional<class RecipeUnlockingRequirement> getRecipeUnlockingRequirement() { return {}; }

    virtual SemVersion getSemVersion() { return SemVersion(1, 20, 50, "", ""); }
};

} // namespace GMLIB::Mod