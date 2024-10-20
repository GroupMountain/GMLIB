#pragma once
#include "Macros.h"
#include "gmlib/mod/recipe/base/CustomFurnaceRecipe.h"
#include "gmlib/mod/recipe/base/CustomShapedRecipe.h"
#include "gmlib/mod/recipe/base/CustomShapelessRecipe.h"
#include "gmlib/mod/recipe/base/CustomShulkerBoxRecipe.h"
#include "ll/api/service/Bedrock.h"

namespace gmlib::mod::recipe {

class RecipeRegistry {
public:
    template <class T>
        requires std::is_base_of<ICustomShapedRecipe, T>::value
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
        requires std::is_base_of<ICustomShapelessRecipe, T>::value
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
        requires std::is_base_of<ICustomFurnaceRecipe, T>::value
    inline static void registerFurnaceRecipe() {
        SharedPtr<T> FurnaceRecipe = SharedPtr<T>::make();
        return ll::service::bedrock::getLevel()->getRecipes().addFurnaceRecipeAuxData(
            FurnaceRecipe->getInput(),
            FurnaceRecipe->getResult(),
            FurnaceRecipe->getCraftingTags()
        );
    }

    template <class T>
        requires std::is_base_of<ICustomShulkerBoxRecipe, T>::value
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
    GMLIB_API static void registerShapelessCraftingTableRecipe(
        std::string const&                                                                        recipe_id,
        std::vector<Recipes::Type> const&                                                         ingredients,
        ItemStack const&                                                                          result,
        std::variant<RecipeUnlockingRequirement::UnlockingContext, std::vector<RecipeIngredient>> unlock =
            RecipeUnlockingRequirement::UnlockingContext::AlwaysUnlocked,
        int priority = 50
    );

    GMLIB_API static void registerStoneCutterRecipe(
        std::string const&                                                                        recipe_id,
        Recipes::Type const&                                                                      input,
        ItemStack const&                                                                          result,
        std::variant<RecipeUnlockingRequirement::UnlockingContext, std::vector<RecipeIngredient>> unlock =
            RecipeUnlockingRequirement::UnlockingContext::AlwaysUnlocked,
        int priority = 50
    );

    GMLIB_API static void registerShapelessCraftingTableRecipe(
        std::string const&                                                                        recipe_id,
        std::vector<std::string> const&                                                           ingredients,
        std::string const&                                                                        result,
        int                                                                                       count,
        std::variant<RecipeUnlockingRequirement::UnlockingContext, std::vector<RecipeIngredient>> unlock =
            RecipeUnlockingRequirement::UnlockingContext::AlwaysUnlocked,
        int priority = 50
    );

    GMLIB_API static void registerShapedCraftingTableRecipe(
        std::string const&                                                                        recipe_id,
        std::vector<std::string> const&                                                           shape,
        std::vector<Recipes::Type> const&                                                         ingredients,
        ItemStack const&                                                                          result,
        std::variant<RecipeUnlockingRequirement::UnlockingContext, std::vector<RecipeIngredient>> unlock =
            RecipeUnlockingRequirement::UnlockingContext::AlwaysUnlocked,
        int priority = 50
    );

    GMLIB_API static void registerShapedCraftingTableRecipe(
        std::string const&                                                                        recipe_id,
        std::vector<std::string> const&                                                           shape,
        std::vector<std::string> const&                                                           ingredients,
        std::string const&                                                                        result,
        int                                                                                       count,
        std::variant<RecipeUnlockingRequirement::UnlockingContext, std::vector<RecipeIngredient>> unlock =
            RecipeUnlockingRequirement::UnlockingContext::AlwaysUnlocked,
        int priority = 50
    );
};

} // namespace gmlib::mod::recipe