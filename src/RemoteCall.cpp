#include <RemoteCallAPI.h>

/*
#include <GMLib/GMLib_ModAPI.h>
std::unordered_set<int> ExperimentsList = {6, 7, 8, 9, 10, 12, 15};

void ExportGMLib_ModAPI() {
    //合成表部分
    RemoteCall::exportAs("GMLib_ModAPI", "registerShapelessRecipe", [](std::string recipe_id, std::vector<std::string> ingredients, std::string result, int count, std::string unlock) -> void { 
        std::vector<RecipeItem> types;
        for (auto ing : ingredients) {
            RecipeItem key = {ing};
            types.push_back(key);
        }
        RecipeItem res = {result, 0, count};
        RecipeItem unl = {unlock};
        GMLib_Mod::addShapelessCraftingTableRecipe(recipe_id, types, res, {unl});
    });
    RemoteCall::exportAs("GMLib_ModAPI", "registerShapedRecipe", [](std::string recipe_id, std::vector<std::string> shape, std::vector<std::string> ingredients, std::string result, int count, std::string unlock) -> void { 
        std::vector<RecipeItem> types;
        for (auto ing : ingredients) {
            RecipeItem key = {ing};
            types.push_back(key);
        }
        RecipeItem res = {result, 0, count};
        RecipeItem unl = {unlock};
        GMLib_Mod::addShapedCraftingTableRecipe(recipe_id, shape, types, res, {unl});
    });
    RemoteCall::exportAs("GMLib_ModAPI", "registerFurnaceRecipe", [](std::string recipe_id, std::string input, std::string output, std::vector<std::string> tags) -> void {
        RecipeItem inp = {input};
        RecipeItem outp = {output};
        GMLib_Mod::addFurnaceRecipe(recipe_id, inp, outp, tags);
    });
    RemoteCall::exportAs("GMLib_ModAPI", "registerBrewingMixRecipe", [](std::string recipe_id, std::string input, std::string output, std::string reagent) -> void { 
        RecipeItem rea = {reagent};
        GMLib_Mod::addBrewingMixRecipe(recipe_id, input, output, rea);
    });
    RemoteCall::exportAs("GMLib_ModAPI", "registerBrewingContainerRecipe", [](std::string recipe_id, std::string input, std::string output, std::string reagent) -> void { 
        RecipeItem inp = {input};
        RecipeItem outp = {output};
        RecipeItem rea = {reagent};
        GMLib_Mod::addBrewingContainerRecipe(recipe_id, inp, outp, rea);
    });
    RemoteCall::exportAs("GMLib_ModAPI", "registerSmithingTransformRecipe", [](std::string recipe_id, std::string smithing_template, std::string base, std::string addition, std::string result) -> void { 
        GMLib_Mod::addSmithingTransformRecipe(recipe_id, smithing_template, base, addition, result);
    });
    RemoteCall::exportAs("GMLib_ModAPI", "registerSmithingTrimRecipe", [](std::string recipe_id, std::string smithing_template, std::string base, std::string addition) -> void { 
        GMLib_Mod::addSmithingTrimRecipe(recipe_id, smithing_template, base, addition);
    });
    RemoteCall::exportAs("GMLib_ModAPI", "registerStoneCutterRecipe", [](std::string recipe_id, std::string input, int input_data, std::string output, int output_data, int output_count) -> void {
        RecipeItem inp = {input, input_data};
        RecipeItem outp = {output, output_data, output_count};
        GMLib_Mod::addStoneCutterRecipe(recipe_id, inp, outp);
    });

    //伤害类型
    RemoteCall::exportAs("GMLib_ModAPI", "registerDamageCause", [](int vanilla_cause, std::string cause_name, std::string direct_killer_type, std::string die_type) -> void { 
        GMLib_Mod::addDamageCause((ActorDamageCause)vanilla_cause, cause_name, direct_killer_type, die_type);
    });
    RemoteCall::exportAs("GMLib_ModAPI", "translateResourcePack", [](std::string key1, std::vector<std::string> key2) -> std::string { 
        return GMLib_Mod::i18nTranslate(key1, key2);
    });

    //物品定义
    RemoteCall::exportAs("GMLib_ModAPI", "addFireImmuneItem", [](std::string id) -> void { 
        GMLib_Mod::addFireImmuneItem(id);
    });
    RemoteCall::exportAs("GMLib_ModAPI", "addExplosionImmuneItem", [](std::string id) -> void { 
        GMLib_Mod::addExplosionImmuneItem(id);
    });
    RemoteCall::exportAs("GMLib_ModAPI", "setArmorToughness", [](std::string id, double value) -> void { 
        GMLib_Mod::addArmorToughnessDefinition(id, value);
    });

    //错误方块清理
    RemoteCall::exportAs("GMLib_ModAPI", "setUnknownBlockCleaner", [](bool value) -> void { 
        GMLib_Mod::setUnknownBlockCleaner(value);
    });

    //生物定义
    RemoteCall::exportAs("GMLib_ModAPI", "setFixUndeadMobs", []() -> void { 
        GMLib_Mod::setFixUndeadMobs();
    });

    //实验性
    RemoteCall::exportAs("GMLib_ModAPI", "registerExperimentsRequire", [](int experiment_id) -> void {
        if (ExperimentsList.count(experiment_id)) {
            GMLib_Mod::addExperimentsRequire((AllExperiments)experiment_id);
        }
        else Logger("Server").error("Experiment ID '{}' does not exist!", experiment_id);
    });
    RemoteCall::exportAs("GMLib_ModAPI", "setExperimentEnabled", [](int experiment_id, bool value) -> void { 
        if (ExperimentsList.count(experiment_id)) {
            GMLib_Mod::setExperimentEnabled(((AllExperiments)experiment_id), value);
        }
        else Logger("Server").error("Experiment ID '{}' does not exist!", experiment_id);
    });
    RemoteCall::exportAs("GMLib_ModAPI", "getExperimentEnabled", [](int experiment_id) -> bool { 
        if (ExperimentsList.count(experiment_id)) {
            return GMLib_Mod::getExperimentEnabled(((AllExperiments)experiment_id));
        }
        else {
            Logger("Server").error("Experiment ID '{}' does not exist!", experiment_id);
            return false;
        }
    });
}
*/

void ExportAPI() {
    //ExportGMLib_ModAPI();
}