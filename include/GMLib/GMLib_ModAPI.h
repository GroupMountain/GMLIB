#pragma once
#include <vector>
#include <mc/Recipes.hpp>
#include <mc/ItemStack.hpp>

#ifdef GMLib_EXPORTS
#define GMLib_ModAPI __declspec(dllexport)
#else
#define GMLib_ModAPI __declspec(dllimport)
#endif

enum class AllExperiments {
    DataDrivenItems = 6,             // 假日创造者功能
    DataDrivenBiomes = 7,            // 自定义生物群系
    UpcomingCreatorFeatures = 8,     // 即将推出的创作者功能
    GameTest = 9,                    // 测试版 API
    MolangFeatures = 10,             // Molang 功能
    Cameras = 12,                    // 实验相机
    VillagerTradesRebalance = 15,    // 村民交易再平衡
};

struct RecipeItem {
    std::string mItem;  // "AlwaysUnlocked", "PlayerHasManyItems", "PlayerInWater", "None", Item ID
    int mData = 0;
    int mCount = 1;
    std::string mType = "item"; // "item", "tag" 仅支持tag合成的可选tag参数
};

class GMLib_Mod {
public:
    //用资源包翻译一条信息
    GMLib_ModAPI static std::string i18nTranslate(std::string key1, std::vector<std::string> key2);
    //设置是否启用错误方块移除
    GMLib_ModAPI static void setUnknownBlockCleaner(bool enable);
    //开关实验性（请在 ServerStartedEvent 之后再使用，如非需要关闭实验性，请尽量不要使用此函数）。
    //不建议直接使用此函数，请使用  直接添加所需实验性依赖，GMLIB 会自动启用需要的实验性。
    GMLib_ModAPI static void setExperimentEnabled(enum class AllExperiments experiment, bool enable);
    //获取实验性是否启用（请在 ServerStartedEvent 之后再使用，模组直接注册实验性依赖项即可，无需获取是否开启）
    GMLib_ModAPI static bool getExperimentEnabled(enum class AllExperiments experiment);
    //添加实验性玩法依赖（注册所需实验性依赖， GMLIB 会自动开启对应的实验性）
    GMLib_ModAPI static void addExperimentsRequire(enum class AllExperiments experiment);

    //自定义合成表API（请在 ServerStartedEvent 之后再使用）
    //json格式加载，不同于Addon里面的json格式，不建议使用
    //如需加载Addon格式请用 loadRecipeJson(std::string json_string)
    GMLib_ModAPI static void addRecipeJson(std::string ricipe_type, std::string json_string);
    //工作台合成表
    GMLib_ModAPI static void addShapelessCraftingTableRecipe(std::string recipe_id, std::vector<RecipeItem> ingredients, RecipeItem result, std::vector<RecipeItem> unlock = {RecipeItem("AlwaysUnlocked")}, int priority = 2);
    GMLib_ModAPI static void addShapedCraftingTableRecipe(std::string recipe_id, std::vector<std::string> shape, std::vector<std::pair<std::string, RecipeItem>> ingredients, RecipeItem result, std::vector<RecipeItem> unlock = {RecipeItem("AlwaysUnlocked")}, int priority = 2);
    GMLib_ModAPI static void addShapedCraftingTableRecipe(std::string recipe_id, std::vector<std::string> shape, std::vector<RecipeItem> ingredients, RecipeItem result, std::vector<RecipeItem> unlock = {RecipeItem("AlwaysUnlocked")}, int priority = 2);
    GMLib_ModAPI static void addLockedShapelessCraftingTableRecipe(std::string recipe_id, std::vector<Recipes::Type> ingredients, ItemStack* result, int priority = 2);
    GMLib_ModAPI static void addLockedShapelessCraftingTableRecipe(std::string recipe_id, std::vector<std::string> ingredients, std::string result, int count = 1, int priority = 2);
    GMLib_ModAPI static void addLockedShapedCraftingTableRecipe(std::string recipe_id, std::vector<std::string> shape, std::vector<Recipes::Type> ingredients, ItemStack* result, int priority = 2);
    GMLib_ModAPI static void addLockedShapedCraftingTableRecipe(std::string recipe_id, std::vector<std::string> shape, std::vector<std::string> ingredients, std::string result, int count = 1, int priority = 2);
    //熔炉/营火合成表
    //tags可填{"furnace", "blast_furnace", "smoker", "campfire", "soul_campfire"}
    GMLib_ModAPI static void addFurnaceRecipe(std::string recipe_id, RecipeItem input, RecipeItem ouput, std::vector<std::string> tags, std::vector<RecipeItem> unlock = {RecipeItem("AlwaysUnlocked")});
    //药水酿造合成表
    GMLib_ModAPI static void addBrewingMixRecipe(std::string recipe_id, std::string input, std::string output, RecipeItem reagent);
    //酿造台合成表
    GMLib_ModAPI static void addBrewingContainerRecipe(std::string recipe_id, RecipeItem input, RecipeItem output, RecipeItem reagent, std::vector<RecipeItem> unlock = {RecipeItem("AlwaysUnlocked")});
    //锻造升级合成表(addition 仅支持下界合金锭)
    GMLib_ModAPI static void addSmithingTransformRecipe(std::string recipe_id, std::string smithing_template, std::string base, std::string addition, std::string result);
    //锻造纹饰合成表
    GMLib_ModAPI static void addSmithingTrimRecipe(std::string recipe_id, std::string smithing_template, std::string base, std::string addition);
    //切石机合成表
    GMLib_ModAPI static void addLockedStoneCutterRecipe(std::string recipe_id, Recipes::Type input, ItemStack* output, int priority = 2);
    GMLib_ModAPI static void addStoneCutterRecipe(std::string recipe_id, RecipeItem input, RecipeItem output, std::vector<RecipeItem> unlock = {RecipeItem("AlwaysUnlocked")}, int priority = 2);
    //自定义合成台Tag合成表
    GMLib_ModAPI static void addCustomCraftingTagRecipe(std::string recipe_id, std::vector<RecipeItem> ingredients, RecipeItem result, std::vector<std::string> tags, std::vector<RecipeItem> unlock = {RecipeItem("AlwaysUnlocked")}, int priority = 2);
    //读取Json合成表
    GMLib_ModAPI static void loadRecipeJson(std::string json_string);
    GMLib_ModAPI static void loadRecipeJson(nlohmann::json json);
    
    //自定义物品API
    //定义物品盔甲韧性
    GMLib_ModAPI static void addArmorToughnessDefinition(std::string item_id, double toughness);
    //定义物品抗火
    GMLib_ModAPI static void addFireImmuneItem(std::string item_id);
    //定义物品防爆
    GMLib_ModAPI static void addExplosionImmuneItem(std::string item_id);

    //控制台输出死亡信息
    GMLib_ModAPI static void setEnableDeathLog(bool logToFile = false);
    //修复武器名称
    GMLib_ModAPI static void setFixWeaponName();
    //自定义伤害类型（这会影响死亡信息）
    GMLib_ModAPI static void addDamageCause(ActorDamageCause vanilla_cause, std::string cause_name, std::string direct_killer_type = "default", std::string die_type = "default");
    GMLib_ModAPI static void addDamageCauseWithMessage(ActorDamageCause vanilla_cause, std::string cause_name, std::string custom_message, std::string direct_killer_type = "default", std::string die_type = "default");

    //修复亡灵生物特性
    GMLib_ModAPI static void setFixUndeadMobs();
    //定义伤害倒置生物（类似亡灵生物）
    GMLib_ModAPI static void addInvertedHealAndHarmMob(std::string id);
};