#pragma once
#include "GMLIB/Macros.h"
#include "mc/codebuilder/MCRESULT.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/actor/player/PlayerScoreSetFunction.h"
#include "mc/world/effect/MobEffect.h"
#include "mc/world/level/biome/Biome.h"

class GMLIB_Actor : public Actor {
public:
    using Actor::addEffect;
    using Actor::getAllEffects;
    using Actor::removeEffect;

public:
    GMLIB_API static int64_t getNextActorUniqueID();

    GMLIB_API static std::vector<ActorUniqueID> getAllEntities();

    GMLIB_API static std::unordered_map<ActorUniqueID, std::string> getActorIdsKeyMap();

    GMLIB_API static ActorUniqueID getActorUniqueID(std::string& actorKey);

    GMLIB_API static std::string getActorTypeName(ActorUniqueID& uniqueId);

    GMLIB_API static std::string getActorTypeName(std::string& actorKey);

    GMLIB_API static std::optional<std::pair<Vec3, DimensionType>> getActorPosition(ActorUniqueID& uniqueId);

    GMLIB_API static std::optional<std::pair<Vec3, DimensionType>> getActorPosition(std::string& actorKey);

    GMLIB_API static bool setActorPosition(ActorUniqueID& uniqueId, Vec3 pos, DimensionType dimId);

    GMLIB_API static bool setActorPosition(std::string& actorKey, Vec3 pos, DimensionType dimId);

    GMLIB_API static std::unique_ptr<CompoundTag> getActorNbt(ActorUniqueID& uniqueId);

    GMLIB_API static std::unique_ptr<CompoundTag> getActorNbt(std::string& actorKey);

    GMLIB_API static bool setActorNbt(ActorUniqueID& uniqueId, CompoundTag& nbt);

    GMLIB_API static bool setActorNbt(std::string& actorKey, CompoundTag& nbt);

    GMLIB_API static bool setActorNbtTags(ActorUniqueID& uniqueId, CompoundTag& nbt, const std::vector<std::string>& tags);

    GMLIB_API static bool
    setActorNbtTags(std::string& actorKey, CompoundTag& nbt, const std::vector<std::string>& tags);

    GMLIB_API static bool deleteActor(ActorUniqueID& uniqueId);

    GMLIB_API static bool deleteActor(std::string& actorKey);

    GMLIB_API static std::optional<int> getActorScore(ActorUniqueID& uniqueId, std::string objective);

    GMLIB_API static std::optional<int> getActorScore(std::string& actorKey, std::string objective);

    GMLIB_API static std::optional<int> setActorScore(
        ActorUniqueID&         uniqueId,
        std::string            objective,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API static std::optional<int> setActorScore(
        std::string&           actorKey,
        std::string            objective,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API static bool resetActorScore(ActorUniqueID& uniqueId, std::string objective);

    GMLIB_API static bool resetActorScore(std::string& actorKey, std::string objective);

    GMLIB_API static bool resetActorScore(ActorUniqueID& uniqueId);

    GMLIB_API static bool resetActorScore(std::string& actorKey);

public:
    GMLIB_API bool isPlayer() const;

    GMLIB_API bool isItemActor() const;

    GMLIB_API bool isMob() const;

    GMLIB_API std::unique_ptr<CompoundTag> getNbt();

    GMLIB_API bool setNbt(CompoundTag& nbt);

    GMLIB_API bool setNbtTags(CompoundTag& nbt, const std::vector<std::string>& tags);

    GMLIB_API std::optional<int> getScore(std::string objective);

    GMLIB_API std::optional<int>
              setScore(std::string objective, int value, PlayerScoreSetFunction action = PlayerScoreSetFunction::Set);

    GMLIB_API bool resetScore(std::string objective);

    GMLIB_API bool resetScore();

    GMLIB_API void addEffect(
        MobEffect::EffectType effectType,
        int                   duration      = 600,
        int                   amplifier     = 0,
        bool                  showParticles = true,
        bool                  ambient       = false,
        bool                  showAnimation = false
    );

    GMLIB_API void removeEffect(MobEffect::EffectType effectType);

    GMLIB_API std::vector<MobEffectInstance> getAllEffects();

    GMLIB_API bool setProjectile(Actor* projectile, float speed = 2, float offset = 3);

    GMLIB_API GMLIB_Actor* shootProjectile(std::string typeName, float speed = 2, float offset = 3);

    GMLIB_API bool throwEntity(Actor* projectile, float speed = 2, float offset = 3);

    GMLIB_API void setHealth(int value);

    GMLIB_API void setMaxHealth(int value);

    GMLIB_API void setAttackDamage(int value);

    GMLIB_API void setFollowRange(int value);

    GMLIB_API void setMaxFollowRange(int value);

    GMLIB_API void setJumpStrength(int value);

    GMLIB_API void setKnockbackResistance(int value);

    GMLIB_API void setLavaMovementSpeed(int value);

    GMLIB_API void setMovementSpeed(int value);

    GMLIB_API void setUnderwaterMovementSpeed(int value);

    GMLIB_API ItemStack* getMainHandSlot();

    GMLIB_API void setMainHandSlot(ItemStack& itemStack);

    GMLIB_API ItemStack* getOffHandSlot();

    GMLIB_API void setOffHandSlot(ItemStack& itemStack);

    GMLIB_API void hurtEntity(float damage, std::string causeName = "override", Actor* source = nullptr);

    GMLIB_API Biome* getBiome();

    GMLIB_API MCRESULT executeCommand(std::string_view command);
};