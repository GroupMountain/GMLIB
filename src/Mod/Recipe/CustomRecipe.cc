#include "GMLIB/Mod/CustomRecipe.h"
#include "Global.h"

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

template <class T>
    requires std::is_base_of<CustomShapedRecipe, T>::value
void CustomRecipe::registerShapedRecipe() {
    SharedPtr<T> ShapedRecipe = SharedPtr<T>::makeShared();
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
void CustomRecipe::registerShapelessRecipe() {
    SharedPtr<T> ShapelessRecipe = SharedPtr<T>::makeShared();
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
void CustomRecipe::registerFurnaceRecipe() {
    SharedPtr<T> FurnaceRecipe = SharedPtr<T>::makeShared();
    return ll::service::bedrock::getLevel()->getRecipes().addFurnaceRecipeAuxData(
        FurnaceRecipe->getInput(),
        FurnaceRecipe->getResult(),
        FurnaceRecipe->getCraftingTags()
    );
}

template <class T>
    requires std::is_base_of<CustomShulkerBoxRecipe, T>::value
void CustomRecipe::registerShulkerBoxRecipe() {
    SharedPtr<T> ShulkerBoxRecipe = SharedPtr<T>::makeShared();
    return ll::service::bedrock::getLevel()->getRecipes().addShulkerBoxRecipe(
        ShulkerBoxRecipe->getRecipeId(),
        ShulkerBoxRecipe->getResult(),
        ShulkerBoxRecipe->getIngredients(),
        ShulkerBoxRecipe->getCraftingTags(),
        ShulkerBoxRecipe->getRecipeUnlockingRequirement(),
        ShulkerBoxRecipe->getSemVersion()
    );
}

bool CustomRecipe::unregisterRecipe(std::string recipe_id) {
    auto AllRecipes = ll::service::bedrock::getLevel()->getRecipes().getRecipesAllTags();
    for (auto& recipe : AllRecipes) {
        if (recipe.second.count(recipe_id)) {
            recipe.second.erase(recipe_id);
            CraftingDataPacket::prepareFromRecipes(ll::service::bedrock::getLevel()->getRecipes(), true)
                ->sendToClients();
            return true;
        }
    }
    return false;
}

} // namespace GMLIB::Mod