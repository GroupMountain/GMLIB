#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/service/Bedrock.h"
#include "mc/world/item/crafting/Recipes.h"
#include "nlohmann/json.hpp"

namespace GMLIB::Mod {

class JsonRecipe {
public:
    GMLIB_API static bool loadJsonRecipe(std::string json);

    GMLIB_API static bool loadJsonRecipe(nlohmann::json json);

    GMLIB_API static bool registerFurnaceRecipe(
        std::string                                              recipeId,
        RecipeIngredient                                         input,
        RecipeIngredient                                         output,
        std::vector<std::string>                                 craftingTags = {"furnace"},
        std::variant<std::string, std::vector<RecipeIngredient>> unlock       = "AlwaysUnlocked"
    );

    GMLIB_API static bool
    registerBrewingMixRecipe(std::string recipeId, std::string input, std::string output, RecipeIngredient reagent);

    GMLIB_API static bool registerBrewingContainerRecipe(
        std::string                                              recipeId,
        RecipeIngredient                                         input,
        RecipeIngredient                                         output,
        RecipeIngredient                                         reagent,
        std::variant<std::string, std::vector<RecipeIngredient>> unlock = "AlwaysUnlocked"
    );

    GMLIB_API static bool registerSmithingTransformRecipe(
        std::string recipeId,
        std::string smithingTemplate,
        std::string base,
        std::string addition,
        std::string result
    );

    GMLIB_API static bool registerSmithingTrimRecipe(
        std::string recipeId,
        std::string smithingTemplate,
        std::string base,
        std::string addition
    );

    GMLIB_API static bool registerStoneCutterRecipe(
        std::string                                              recipeId,
        RecipeIngredient                                         input,
        RecipeIngredient                                         output,
        std::variant<std::string, std::vector<RecipeIngredient>> unlock   = "AlwaysUnlocked",
        int                                                      priority = 50
    );

    GMLIB_API static bool registerCustomCraftingTagRecipe(
        std::string                                              recipeId,
        std::vector<RecipeIngredient>                            ingredients,
        RecipeIngredient                                         result,
        std::vector<std::string>                                 tags,
        std::variant<std::string, std::vector<RecipeIngredient>> unlock   = "AlwaysUnlocked",
        int                                                      priority = 50
    );

    GMLIB_API static bool registerShapelessCraftingTableRecipe(
        std::string                                              recipeId,
        std::vector<RecipeIngredient>                            ingredients,
        RecipeIngredient                                         result,
        std::variant<std::string, std::vector<RecipeIngredient>> unlock   = "AlwaysUnlocked",
        int                                                      priority = 50
    );

    GMLIB_API static bool registerShapedCraftingTableRecipe(
        std::string                                              recipeId,
        std::vector<std::string>                                 shape,
        std::vector<std::pair<std::string, RecipeIngredient>>    ingredients,
        RecipeIngredient                                         result,
        std::variant<std::string, std::vector<RecipeIngredient>> unlock   = "AlwaysUnlocked",
        int                                                      priority = 50
    );

    GMLIB_API static bool registerShapedCraftingTableRecipe(
        std::string                                              recipeId,
        std::vector<std::string>                                 shape,
        std::vector<RecipeIngredient>                            ingredients,
        RecipeIngredient                                         result,
        std::variant<std::string, std::vector<RecipeIngredient>> unlock   = "AlwaysUnlocked",
        int                                                      priority = 50
    );
};

} // namespace GMLIB::Mod