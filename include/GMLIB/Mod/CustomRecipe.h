#pragma once
#include "GMLIB/GMLIB.h"
#include "GMLIB/Mod/Recipe/CustomFurnaceRecipe.h"
#include "GMLIB/Mod/Recipe/CustomShapedRecipe.h"
#include "GMLIB/Mod/Recipe/CustomShapelessRecipe.h"
#include "GMLIB/Mod/Recipe/CustomShulkerBoxRecipe.h"
#include <nlohmann/json.hpp>
#include "ll/api/service/Bedrock.h"

namespace GMLIB::Mod {

class CustomRecipe {
public:
    template <class T>
        requires std::is_base_of<CustomShapedRecipe, T>::value
    inline static void registerShapedRecipe() {
        SharedPtr<T> ShapedRecipe = SharedPtr<T>::make();
        return ll::service::bedrock::getLevel()->getRecipes().addShapedRecipe(
            ShapedRecipe->getRecipeId(),
            ShapedRecipe->getResult(),
            ShapedRecipe->getShape(),
            ShapedRecipe->getIngredients(),
            ShapedRecipe->getCraftingTags(),
            ShapedRecipe->getPriority(),
            ShapedRecipe->getConstructor(),
            ShapedRecipe->getRecipeUnlockingRequirement(),
            ShapedRecipe->getSemVersion()
        );
    }

    template <class T>
        requires std::is_base_of<CustomShapelessRecipe, T>::value
    inline static void registerShapelessRecipe() {
        SharedPtr<T> ShapelessRecipe = SharedPtr<T>::make();
        return ll::service::bedrock::getLevel()->getRecipes().addShapelessRecipe(
            ShapelessRecipe->getRecipeId(),
            ShapelessRecipe->getResult(),
            ShapelessRecipe->getIngredients(),
            ShapelessRecipe->getCraftingTags(),
            ShapelessRecipe->getPriority(),
            ShapelessRecipe->getConstructor(),
            ShapelessRecipe->getRecipeUnlockingRequirement(),
            ShapelessRecipe->getSemVersion()
        );
    }

    template <class T>
        requires std::is_base_of<CustomFurnaceRecipe, T>::value
    inline static void registerFurnaceRecipe() {
        SharedPtr<T> FurnaceRecipe = SharedPtr<T>::make();
        return ll::service::bedrock::getLevel()->getRecipes().addFurnaceRecipeAuxData(
            FurnaceRecipe->getInput(),
            FurnaceRecipe->getResult(),
            FurnaceRecipe->getCraftingTags()
        );
    }

    template <class T>
        requires std::is_base_of<CustomShulkerBoxRecipe, T>::value
    inline static void registerShulkerBoxRecipe() {
        SharedPtr<T> ShulkerBoxRecipe = SharedPtr<T>::make();
        return ll::service::bedrock::getLevel()->getRecipes().addShulkerBoxRecipe(
            ShulkerBoxRecipe->getRecipeId(),
            ShulkerBoxRecipe->getResult(),
            ShulkerBoxRecipe->getIngredients(),
            ShulkerBoxRecipe->getCraftingTags(),
            ShulkerBoxRecipe->getRecipeUnlockingRequirement(),
            ShulkerBoxRecipe->getSemVersion()
        );
    }

    GMLIB_API static bool unregisterRecipe(std::string recipe_id);

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

    GMLIB_API static void registerLockedShapelessCraftingTableRecipe(
        std::string                recipe_id,
        std::vector<Recipes::Type> ingredients,
        ItemStack*                 result,
        int                        priority = 50
    );

    GMLIB_API static void
    registerLockedStoneCutterRecipe(std::string recipe_id, Recipes::Type input, ItemStack* result, int priority);

    GMLIB_API static void registerLockedShapelessCraftingTableRecipe(
        std::string              recipe_id,
        std::vector<std::string> ingredients,
        std::string              result,
        int                      count,
        int                      priority = 50
    );

    GMLIB_API static void registerLockedShapedCraftingTableRecipe(
        std::string                recipe_id,
        std::vector<std::string>   shape,
        std::vector<Recipes::Type> ingredients,
        ItemStack*                 result,
        int                        priority = 50
    );

    GMLIB_API static void registerLockedShapedCraftingTableRecipe(
        std::string              recipe_id,
        std::vector<std::string> shape,
        std::vector<std::string> ingredients,
        std::string              result,
        int                      count,
        int                      priority = 50
    );
};

} // namespace GMLIB::Mod