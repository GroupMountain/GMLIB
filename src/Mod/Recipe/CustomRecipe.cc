#include "Global.h"
#include <GMLIB/Mod/CustomRecipe/CustomRecipe.h>
#include <GMLIB/Server/LevelAPI.h>
#include <mc/deps/core/sem_ver/SemVersion.h>
#include <mc/network/packet/CraftingDataPacket.h>

using RecipesMap = std::map<
    class HashedString,
    class std::map<
        std::string,
        class std::shared_ptr<class Recipe>,
        struct std::less<std::string>,
        class std::allocator<struct std::pair<std::string const, class std::shared_ptr<class Recipe>>>>,
    struct std::less<class HashedString>,
    class std::allocator<struct std::pair<
        class HashedString const,
        class std::map<
            std::string,
            class std::shared_ptr<class Recipe>,
            struct std::less<std::string>,
            class std::allocator<struct std::pair<std::string const, class std::shared_ptr<class Recipe>>>>>>>;


namespace GMLIB::Mod {

bool CustomRecipe::unregisterRecipe(std::string recipe_id) {
    auto AllRecipes = ll::service::bedrock::getLevel()->getRecipes().getRecipesAllTags();
    for (auto& recipe : AllRecipes) {
        if (recipe.second.count(recipe_id)) {
            recipe.second.erase(recipe_id);
            auto packet = CraftingDataPacket::prepareFromRecipes(ll::service::bedrock::getLevel()->getRecipes(), true);
            GMLIB_Level::getInstance()->sendPacketToClients(*packet);
            return true;
        }
    }
    return false;
}

RecipeUnlockingRequirement makeRecipeUnlockingRequirement(
    std::variant<RecipeUnlockingRequirement::UnlockingContext, std::vector<RecipeIngredient>>& unlock
) {
    if (std::holds_alternative<RecipeUnlockingRequirement::UnlockingContext>(unlock)) {
        auto context    = std::get<RecipeUnlockingRequirement::UnlockingContext>(unlock);
        auto result     = RecipeUnlockingRequirement();
        result.mContext = context;
        result.mRule    = RecipeUnlockingRequirement::UnlockingRule::ByContext;
        return result;
    } else {
        auto unlocks = std::get<std::vector<RecipeIngredient>>(unlock);
        return RecipeUnlockingRequirement(unlocks);
    }
}

void CustomRecipe::registerShapelessCraftingTableRecipe(
    std::string const&                                                                        recipe_id,
    std::vector<Recipes::Type> const&                                                         ingredients,
    ItemStack*                                                                                result,
    std::variant<RecipeUnlockingRequirement::UnlockingContext, std::vector<RecipeIngredient>> unlock,
    int                                                                                       priority
) {
    if (ingredients.size() == 0 || ingredients.size() >= 10) {
        return;
    }
    ll::service::bedrock::getLevel()->getRecipes().addShapelessRecipe(
        recipe_id,
        ItemInstance(*result),
        ingredients,
        {"crafting_table"},
        priority,
        nullptr,
        makeRecipeUnlockingRequirement(unlock),
        SemVersion(1, 20, 80, "", "")
    );
}

void CustomRecipe::registerStoneCutterRecipe(
    std::string const&                                                                        recipe_id,
    Recipes::Type const&                                                                      input,
    ItemStack*                                                                                result,
    std::variant<RecipeUnlockingRequirement::UnlockingContext, std::vector<RecipeIngredient>> unlock,
    int                                                                                       priority
) {
    std::vector<Recipes::Type> types = {input};
    ll::service::bedrock::getLevel()->getRecipes().addShapelessRecipe(
        recipe_id,
        ItemInstance(*result),
        types,
        {"stonecutter"},
        priority,
        nullptr,
        makeRecipeUnlockingRequirement(unlock),
        SemVersion(1, 20, 80, "", "")
    );
}

void CustomRecipe::registerShapelessCraftingTableRecipe(
    std::string const&                                                                        recipe_id,
    std::vector<std::string> const&                                                           ingredients,
    std::string const&                                                                        result,
    int                                                                                       count,
    std::variant<RecipeUnlockingRequirement::UnlockingContext, std::vector<RecipeIngredient>> unlock,
    int                                                                                       priority
) {
    if (ingredients.size() == 0 || ingredients.size() >= 10) {
        return;
    }
    auto                       item  = ItemStack{result, count};
    std::vector<Recipes::Type> types = {};
    for (int i = 0; i <= ingredients.size() - 1; i++) {
        auto key  = (char)(i + 65);
        auto type = Recipes::Type(ingredients[i], key, 1, 0);
        types.push_back(type);
    }
    ll::service::bedrock::getLevel()->getRecipes().addShapelessRecipe(
        recipe_id,
        ItemInstance(item),
        types,
        {"crafting_table"},
        priority,
        nullptr,
        makeRecipeUnlockingRequirement(unlock),
        SemVersion(1, 20, 80, "", "")
    );
}

void CustomRecipe::registerShapedCraftingTableRecipe(
    std::string const&                                                                        recipe_id,
    std::vector<std::string> const&                                                           shape,
    std::vector<Recipes::Type> const&                                                         ingredients,
    ItemStack*                                                                                result,
    std::variant<RecipeUnlockingRequirement::UnlockingContext, std::vector<RecipeIngredient>> unlock,
    int                                                                                       priority
) {
    if (ingredients.size() == 0 || ingredients.size() >= 10) {
        return;
    }
    ll::service::bedrock::getLevel()->getRecipes().addShapedRecipe(
        recipe_id,
        ItemInstance(*result),
        shape,
        ingredients,
        {"crafting_table"},
        priority,
        nullptr,
        makeRecipeUnlockingRequirement(unlock),
        SemVersion(1, 20, 80, "", ""),
        true
    );
}

void CustomRecipe::registerShapedCraftingTableRecipe(
    std::string const&                                                                        recipe_id,
    std::vector<std::string> const&                                                           shape,
    std::vector<std::string> const&                                                           ingredients,
    std::string const&                                                                        result,
    int                                                                                       count,
    std::variant<RecipeUnlockingRequirement::UnlockingContext, std::vector<RecipeIngredient>> unlock,
    int                                                                                       priority
) {
    if (ingredients.size() == 0 || ingredients.size() >= 10) {
        return;
    }
    auto                       item  = ItemStack{result, count};
    std::vector<Recipes::Type> types = {};
    for (int i = 0; i <= ingredients.size() - 1; i++) {
        auto key  = (char)(i + 65);
        auto type = Recipes::Type(ingredients[i], key, 1, 0);
        types.push_back(type);
    }
    ll::service::bedrock::getLevel()->getRecipes().addShapedRecipe(
        recipe_id,
        ItemInstance(item),
        shape,
        types,
        {"crafting_table"},
        priority,
        nullptr,
        makeRecipeUnlockingRequirement(unlock),
        SemVersion(1, 20, 80, "", ""),
        true
    );
}

} // namespace GMLIB::Mod