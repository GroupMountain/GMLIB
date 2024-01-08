#pragma once
#include "GMLIB/DllExport.h"
#include "GMLIB/Mod/Recipe/CustomFurnaceRecipe.h"
#include "GMLIB/Mod/Recipe/CustomShapedRecipe.h"
#include "GMLIB/Mod/Recipe/CustomShapelessRecipe.h"
#include "GMLIB/Mod/Recipe/CustomShulkerBoxRecipe.h"

namespace GMLIB::Mod::Recipe {

template <class T>
    requires std::is_base_of<CustomShapedRecipe, T>::value
GMLIB_API void registerShapedRecipe();

template <class T>
    requires std::is_base_of<CustomShapelessRecipe, T>::value
GMLIB_API void registerShapelessRecipe();

template <class T>
    requires std::is_base_of<CustomFurnaceRecipe, T>::value
GMLIB_API void registerFurnaceRecipe();

template <class T>
    requires std::is_base_of<CustomShulkerBoxRecipe, T>::value
GMLIB_API void registerShulkerBoxRecipe();

GMLIB_API bool unregisterRecipe(std::string recipe_id);

namespace RapidRecipeLoader {

GMLIB_API bool loadJsonRecipe(std::string json);
GMLIB_API bool loadJsonRecipe(nlohmann::json json);

GMLIB_API void addFurnaceRecipe(
    std::string                                              recipeId,
    RecipeIngredient                                         input,
    RecipeIngredient                                         output,
    std::vector<std::string>                                 craftingTags,
    std::variant<std::string, std::vector<RecipeIngredient>> unlock
);

GMLIB_API void
addBrewingMixRecipe(std::string recipeId, std::string input, std::string output, RecipeIngredient reagent);

GMLIB_API void addBrewingContainerRecipe(
    std::string                                              recipeId,
    RecipeIngredient                                         input,
    RecipeIngredient                                         output,
    RecipeIngredient                                         reagent,
    std::variant<std::string, std::vector<RecipeIngredient>> unlock
);

GMLIB_API void addSmithingTransformRecipe(
    std::string recipeId,
    std::string smithingTemplate,
    std::string base,
    std::string addition,
    std::string result
);

GMLIB_API void
addSmithingTrimRecipe(std::string recipeId, std::string smithingTemplate, std::string base, std::string addition);

GMLIB_API void addStoneCutterRecipe(
    std::string                                              recipeId,
    RecipeIngredient                                         input,
    RecipeIngredient                                         output,
    std::variant<std::string, std::vector<RecipeIngredient>> unlock,
    int                                                      priority
);

GMLIB_API void addCustomCraftingTagRecipe(
    std::string                                              recipeId,
    std::vector<RecipeIngredient>                            ingredients,
    RecipeIngredient                                         result,
    std::vector<std::string>                                 tags,
    std::variant<std::string, std::vector<RecipeIngredient>> unlock,
    int                                                      priority
);

GMLIB_API void addShapelessCraftingTableRecipe(
    std::string                                              recipeId,
    std::vector<RecipeIngredient>                            ingredients,
    RecipeIngredient                                         result,
    std::variant<std::string, std::vector<RecipeIngredient>> unlock,
    int                                                      priority
);

GMLIB_API void addShapedCraftingTableRecipe(
    std::string                                              recipeId,
    std::vector<std::string>                                 shape,
    std::vector<std::pair<std::string, RecipeIngredient>>    ingredients,
    RecipeIngredient                                         result,
    std::variant<std::string, std::vector<RecipeIngredient>> unlock,
    int                                                      priority
);

GMLIB_API void addShapedCraftingTableRecipe(
    std::string                   recipeId,
    std::vector<std::string>      shape,
    std::vector<RecipeIngredient> ingredients,
    RecipeIngredient              result,
    std::vector<RecipeIngredient> unlock,
    int                           priority
);

} // namespace RapidRecipeLoader

} // namespace GMLIB::Mod::Recipe
