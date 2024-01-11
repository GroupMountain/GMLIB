#include "GMLIB/Mod/CustomRecipe.h"
#include "Global.h"

Json::Reader reader;

// 加载json格式
bool addRecipeJson(std::string ricipe_type, std::string json_string) {
    Json::Value value;
    reader.parse(json_string, value, true);
    return ll::service::bedrock::getLevel()
        ->getRecipes()
        .loadRecipe({ricipe_type, value}, SemVersion(1, 20, 50, "", ""), SemVersion(1, 20, 50, "", ""), true);
}

// json形式构造
bool GMLIB_CustomRecipe::loadJsonRecipe(std::string json) {
    Json::Value value;
    reader.parse(json, value, true);
    auto info = ll::service::bedrock::getLevel()->getRecipes().extractRecipeObjInfo(value);
    auto ver  = ll::service::bedrock::getLevel()->getRecipes().extractRecipeFormatVersion(value);
    return ll::service::bedrock::getLevel()->getRecipes().loadRecipe(info, ver, ver, true);
}

bool GMLIB_CustomRecipe::loadJsonRecipe(nlohmann::json json) {
    auto json_string = json.dump(4);
    return loadJsonRecipe(json_string);
}

bool GMLIB_CustomRecipe::registerFurnaceRecipe(
    std::string                                              recipe_id,
    RecipeIngredient                                         input,
    RecipeIngredient                                         output,
    std::vector<std::string>                                 tags,
    std::variant<std::string, std::vector<RecipeIngredient>> unlock
) {
    nlohmann::ordered_json recipe_json = {
        {"description", {{"identifier", recipe_id}}                                     },
        {"tags",        tags                                                            },
        {"input",       {{"item", input.getFullName()}, {"data", input.getAuxValue()}}  },
        {"output",      {{"item", output.getFullName()}, {"data", output.getAuxValue()}}}
    };
    if (std::holds_alternative<std::string>(unlock)) {
        recipe_json["unlock"]["context"] = std::get<std::string>(unlock);
    } else {
        auto unlocks = std::get<std::vector<RecipeIngredient>>(unlock);
        for (auto& key : unlocks) {
            recipe_json["unlock"].push_back({
                {"item", key.getFullName()},
                {"data", key.getAuxValue()}
            });
        }
    }
    return addRecipeJson("recipe_furnace", recipe_json.dump(4));
}

bool GMLIB_CustomRecipe::registerBrewingMixRecipe(
    std::string      recipe_id,
    std::string      input,
    std::string      output,
    RecipeIngredient reagent
) {
    nlohmann::ordered_json recipe_json = {
        {"description", {{"identifier", recipe_id}}                                       },
        {"tags",        {"brewing_stand"}                                                 },
        {"input",       input                                                             },
        {"reagent",     {{"item", reagent.getFullName()}, {"data", reagent.getAuxValue()}}},
        {"output",      output                                                            }
    };
    return addRecipeJson("recipe_brewing_mix", recipe_json.dump(4));
}

bool GMLIB_CustomRecipe::registerBrewingContainerRecipe(
    std::string                                              recipe_id,
    RecipeIngredient                                         input,
    RecipeIngredient                                         output,
    RecipeIngredient                                         reagent,
    std::variant<std::string, std::vector<RecipeIngredient>> unlock
) {
    nlohmann::ordered_json recipe_json = {
        {"description", {{"identifier", recipe_id}}                                       },
        {"tags",        {"brewing_stand"}                                                 },
        {"input",       {{"item", input.getFullName()}, {"data", input.getAuxValue()}}    },
        {"reagent",     {{"item", reagent.getFullName()}, {"data", reagent.getAuxValue()}}},
        {"output",      {{"item", output.getFullName()}, {"data", output.getAuxValue()}}  }
    };
    if (std::holds_alternative<std::string>(unlock)) {
        recipe_json["unlock"]["context"] = std::get<std::string>(unlock);
    } else {
        auto unlocks = std::get<std::vector<RecipeIngredient>>(unlock);
        for (auto& key : unlocks) {
            recipe_json["unlock"].push_back({
                {"item", key.getFullName()},
                {"data", key.getAuxValue()}
            });
        }
    }
    return addRecipeJson("recipe_brewing_container", recipe_json.dump(4));
}

bool GMLIB_CustomRecipe::registerSmithingTransformRecipe(
    std::string recipe_id,
    std::string smithing_template,
    std::string base,
    std::string addition,
    std::string result
) {
    nlohmann::ordered_json recipe_json = {
        {"description", {{"identifier", recipe_id}}},
        {"tags",        {"smithing_table"}         },
        {"template",    smithing_template          },
        {"base",        base                       },
        {"addition",    addition                   },
        {"result",      result                     }
    };
    return addRecipeJson("recipe_smithing_transform", recipe_json.dump(4));
}

bool GMLIB_CustomRecipe::registerSmithingTrimRecipe(
    std::string recipe_id,
    std::string smithing_template,
    std::string base,
    std::string addition
) {
    nlohmann::ordered_json recipe_json = {
        {"description", {{"identifier", recipe_id}}},
        {"tags",        {"smithing_table"}         },
        {"template",    smithing_template          },
        {"base",        base                       },
        {"addition",    addition                   },
    };
    return addRecipeJson("recipe_smithing_trim", recipe_json.dump(4));
}

bool GMLIB_CustomRecipe::registerStoneCutterRecipe(
    std::string                                              recipe_id,
    RecipeIngredient                                         input,
    RecipeIngredient                                         output,
    std::variant<std::string, std::vector<RecipeIngredient>> unlock,
    int                                                      priority
) {
    nlohmann::ordered_json recipe_json = {
        {"description", {{"identifier", recipe_id}}                                                                       },
        {"tags",        {"stonecutter"}                                                                                   },
        {"ingredients", {{{"item", input.getFullName()}, {"data", input.getAuxValue()}}}                                  },
        {"result",      {{"item", output.getFullName()}, {"data", output.getAuxValue()}, {"count", output.getStackSize()}}},
        {"priority",    priority                                                                                          }
    };
    if (std::holds_alternative<std::string>(unlock)) {
        recipe_json["unlock"]["context"] = std::get<std::string>(unlock);
    } else {
        auto unlocks = std::get<std::vector<RecipeIngredient>>(unlock);
        for (auto& key : unlocks) {
            recipe_json["unlock"].push_back({
                {"item", key.getFullName()},
                {"data", key.getAuxValue()}
            });
        }
    }
    return addRecipeJson("recipe_shapeless", recipe_json.dump(4));
}

bool GMLIB_CustomRecipe::registerCustomCraftingTagRecipe(
    std::string                                              recipe_id,
    std::vector<RecipeIngredient>                            ingredients,
    RecipeIngredient                                         result,
    std::vector<std::string>                                 tags,
    std::variant<std::string, std::vector<RecipeIngredient>> unlock,
    int                                                      priority
) {
    if (ingredients.size() == 0) {
        return false;
    }
    nlohmann::ordered_json recipe_json = {
        {"description", {{"identifier", recipe_id}}                                                                       },
        {"tags",        tags                                                                                              },
        {"result",      {{"item", result.getFullName()}, {"data", result.getAuxValue()}, {"count", result.getStackSize()}}},
        {"priority",    priority                                                                                          }
    };
    for (int i = 0; i <= ingredients.size() - 1; i++) {
        recipe_json["ingredients"][i]["item"]  = ingredients[i].getFullName();
        recipe_json["ingredients"][i]["data"]  = ingredients[i].getAuxValue();
        recipe_json["ingredients"][i]["count"] = ingredients[i].getStackSize();
    };
    if (std::holds_alternative<std::string>(unlock)) {
        recipe_json["unlock"]["context"] = std::get<std::string>(unlock);
    } else {
        auto unlocks = std::get<std::vector<RecipeIngredient>>(unlock);
        for (auto& key : unlocks) {
            recipe_json["unlock"].push_back({
                {"item", key.getFullName()},
                {"data", key.getAuxValue()}
            });
        }
    }
    return addRecipeJson("recipe_shapeless", recipe_json.dump(4));
}

bool GMLIB_CustomRecipe::registerShapelessCraftingTableRecipe(
    std::string                                              recipe_id,
    std::vector<RecipeIngredient>                            ingredients,
    RecipeIngredient                                         result,
    std::variant<std::string, std::vector<RecipeIngredient>> unlock,
    int                                                      priority
) {
    return registerCustomCraftingTagRecipe(recipe_id, ingredients, result, {"crafting_table"}, unlock, priority);
}

bool GMLIB_CustomRecipe::registerShapedCraftingTableRecipe(
    std::string                                              recipe_id,
    std::vector<std::string>                                 shape,
    std::vector<std::pair<std::string, RecipeIngredient>>    ingredients,
    RecipeIngredient                                         result,
    std::variant<std::string, std::vector<RecipeIngredient>> unlock,
    int                                                      priority
) {
    if (ingredients.size() == 0) {
        return false;
    }
    nlohmann::ordered_json recipe_json = {
        {"description", {{"identifier", recipe_id}}                                                                       },
        {"tags",        {"crafting_table"}                                                                                },
        {"pattern",     shape                                                                                             },
        {"result",      {{"item", result.getFullName()}, {"data", result.getAuxValue()}, {"count", result.getStackSize()}}}
    };
    if (std::holds_alternative<std::string>(unlock)) {
        recipe_json["unlock"]["context"] = std::get<std::string>(unlock);
    } else {
        auto unlocks = std::get<std::vector<RecipeIngredient>>(unlock);
        for (auto& key : unlocks) {
            recipe_json["unlock"].push_back({
                {"item", key.getFullName()},
                {"data", key.getAuxValue()}
            });
        }
    }
    for (auto ingredient : ingredients) {
        recipe_json["key"][ingredient.first] = {
            {"item", ingredient.second.getFullName()},
            {"data", ingredient.second.getAuxValue()}
        };
    }
    return addRecipeJson("recipe_shaped", recipe_json.dump(4));
}

bool GMLIB_CustomRecipe::registerShapedCraftingTableRecipe(
    std::string                                              recipe_id,
    std::vector<std::string>                                 shape,
    std::vector<RecipeIngredient>                            ingredients,
    RecipeIngredient                                         result,
    std::variant<std::string, std::vector<RecipeIngredient>> unlock,
    int                                                      priority
) {
    if (ingredients.size() == 0) {
        return false;
    }
    std::vector<std::pair<std::string, RecipeIngredient>> types;
    for (int i = 0; i <= ingredients.size() - 1; i++) {
        char letter = 'A' + i;
        auto key    = std::string(1, letter);
        types.push_back({key, ingredients[i]});
    }
    return registerShapedCraftingTableRecipe(recipe_id, shape, types, result, unlock, priority);
}

void registerLockedShapelessCraftingTableRecipe(
    std::string                recipe_id,
    std::vector<Recipes::Type> ingredients,
    ItemStack*                 result,
    int                        priority
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
        {},
        SemVersion(1, 20, 50, "", "")
    );
}

void registerLockedStoneCutterRecipe(std::string recipe_id, Recipes::Type input, ItemStack* result, int priority) {
    std::vector<Recipes::Type> types = {input};
    ll::service::bedrock::getLevel()->getRecipes().addShapelessRecipe(
        recipe_id,
        ItemInstance(*result),
        types,
        {"stonecutter"},
        priority,
        nullptr,
        {},
        SemVersion(1, 20, 50, "", "")
    );
}

void registerLockedShapelessCraftingTableRecipe(
    std::string              recipe_id,
    std::vector<std::string> ingredients,
    std::string              result,
    int                      count,
    int                      priority
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
        {},
        SemVersion(1, 20, 50, "", "")
    );
}

void registerLockedShapedCraftingTableRecipe(
    std::string                recipe_id,
    std::vector<std::string>   shape,
    std::vector<Recipes::Type> ingredients,
    ItemStack*                 result,
    int                        priority
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
        {},
        SemVersion(1, 20, 50, "", "")
    );
}

void registerLockedShapedCraftingTableRecipe(
    std::string              recipe_id,
    std::vector<std::string> shape,
    std::vector<std::string> ingredients,
    std::string              result,
    int                      count,
    int                      priority
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
        {},
        SemVersion(1, 20, 50, "", "")
    );
}