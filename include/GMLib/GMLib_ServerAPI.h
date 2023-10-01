#pragma once
#include <mc/Vec3.hpp>
#include <FormUI.h>
#include <mc/Actor.hpp>

#ifdef GMLib_EXPORTS
#define GMLib_ServerAPI __declspec(dllexport)
#else
#define GMLib_ServerAPI __declspec(dllimport)
#endif

class GMLib_Server {
   public:
    //悬浮字API
    //发送悬浮字数据包，返回值为悬浮字的id
    GMLib_ServerAPI static int sendAddFloatingTextPacket(std::string text, Vec3 pos, int dimid);
    //向单个玩家发送悬浮字数据包
    GMLib_ServerAPI static int sendAddFloatingTextPacketToPlayer(std::string text, Vec3 pos, int dimid, Player* pl);
    //清除悬浮字
    GMLib_ServerAPI static void sendDeleteFloatingTextPacket(int id);
    //动态修改悬浮字
    GMLib_ServerAPI static void sendUpdateFloatingTextPacket(int id, std::string newtext);
    
    //服务器设置API
    GMLib_ServerAPI static void setServerSetting(Form::CustomForm cfm);

    //教育版API
    //开启/关闭教育版
    GMLib_ServerAPI static void setEducationFeatureEnabled(bool value);
    //注册/ability命令（如果开启了教育版，则会自动注册该命令，无需再注册）
    GMLib_ServerAPI static void registerAbilityCommand();

    //世界设置API
    //强行启用成就（会禁用作弊）
    GMLib_ServerAPI static void setEnableAchievement();
    //强制信任每一个皮肤
    GMLib_ServerAPI static void setForceTrustSkins();
    //启用资源包双端共存
    GMLib_ServerAPI static void enableCoResourcePack();
    //获取存档名字
    GMLib_ServerAPI static std::string getLevelName();
    //修改存档名字
    GMLib_ServerAPI static void setLevelName(std::string name);
    //设置假种子
    GMLib_ServerAPI static void setFakeSeed(int64_t seed);

    //实体生成API
    //生成一个实体（任意实体）
    GMLib_ServerAPI static Actor* spawnActor(Vec3 pos, int dimid, std::string name, Actor* owner = nullptr);
    //发射一个弹射物，弹射物可以是任意实体（可以是箭、tnt甚至是苦力怕）
    GMLib_ServerAPI static Actor* shootProjectile(Actor* owner, std::string name, float speed, float offset = 0);
    //投掷一个实体，speed应为正数
    GMLib_ServerAPI static bool throwEntity(Actor* owner, Actor* actor, float speed, float offset = 0);
};