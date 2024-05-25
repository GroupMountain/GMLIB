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
    GMLIB_NDAPI static int64_t getNextActorUniqueID();

    GMLIB_NDAPI static std::vector<ActorUniqueID> getAllEntitiesUniqueID();

    GMLIB_NDAPI static std::vector<std::string> getAllEntitiesStorageKey();

    GMLIB_NDAPI static std::unordered_map<int64, std::string> getActorStorageKeyMap();

    GMLIB_NDAPI static ActorUniqueID getActorUniqueID(std::string const& storageKey);

    GMLIB_NDAPI static std::string getActorStorageKeyFromUniqueId(ActorUniqueID const& uniqueId);

    GMLIB_NDAPI static std::string getActorTypeName(ActorUniqueID const& uniqueId);

    GMLIB_NDAPI static std::string getActorTypeName(std::string const& storageKey);

    GMLIB_NDAPI static std::optional<std::pair<Vec3, DimensionType>> getActorPosition(ActorUniqueID const& uniqueId);

    GMLIB_NDAPI static std::optional<std::pair<Vec3, DimensionType>> getActorPosition(std::string const& storageKey);

    GMLIB_API static bool setActorPosition(ActorUniqueID const& uniqueId, Vec3 const& pos, DimensionType dimId);

    GMLIB_API static bool setActorPosition(std::string const& storageKey, Vec3 const& pos, DimensionType dimId);

    GMLIB_NDAPI static std::unique_ptr<CompoundTag> getActorNbt(ActorUniqueID const& uniqueId);

    GMLIB_NDAPI static std::unique_ptr<CompoundTag> getActorNbt(std::string const& storageKey);

    GMLIB_API static bool setActorNbt(ActorUniqueID const& uniqueId, CompoundTag& nbt);

    GMLIB_API static bool setActorNbt(std::string const& storageKey, CompoundTag& nbt);

    GMLIB_API static bool
    setActorNbtTags(ActorUniqueID const& uniqueId, CompoundTag& nbt, std::vector<std::string> const& tags);

    GMLIB_API static bool
    setActorNbtTags(std::string const& storageKey, CompoundTag& nbt, std::vector<std::string> const& tags);

    GMLIB_API static bool deleteActor(ActorUniqueID const& uniqueId);

    GMLIB_API static bool deleteActor(std::string const& storageKey);

    GMLIB_NDAPI static std::optional<int> getActorScore(ActorUniqueID const& uniqueId, std::string const& objective);

    GMLIB_NDAPI static std::optional<int> getActorScore(std::string const& storageKey, std::string const& objective);

    GMLIB_API static std::optional<int> setActorScore(
        ActorUniqueID const&   uniqueId,
        std::string const&     objective,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API static std::optional<int> setActorScore(
        std::string const&     storageKey,
        std::string const&     objective,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API static bool resetActorScore(ActorUniqueID const& uniqueId, std::string const& objective);

    GMLIB_API static bool resetActorScore(std::string const& storageKey, std::string const& objective);

    GMLIB_API static bool resetActorScore(ActorUniqueID const& uniqueId);

    GMLIB_API static bool resetActorScore(std::string const& storageKey);

public:
    GMLIB_API bool isPlayer() const;

    GMLIB_API bool isItemActor() const;

    GMLIB_API bool isMob() const;

    GMLIB_NDAPI std::string getDimensionName();

    GMLIB_NDAPI std::string getDimensionTypeName();

    GMLIB_NDAPI std::unique_ptr<CompoundTag> getNbt();

    GMLIB_API bool setNbt(CompoundTag& nbt);

    GMLIB_API bool setNbtTags(CompoundTag& nbt, std::vector<std::string> const& tags);

    GMLIB_NDAPI std::optional<int> getScore(std::string const& objective);

    GMLIB_NDAPI std::optional<int>
    setScore(std::string const& objective, int value, PlayerScoreSetFunction action = PlayerScoreSetFunction::Set);

    GMLIB_API bool resetScore(std::string const& objective);

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

    GMLIB_NDAPI std::vector<MobEffectInstance> getAllEffects();

    GMLIB_API bool setProjectile(Actor& projectile, float speed = 2, float offset = 3);

    GMLIB_API optional_ref<Actor> shootProjectile(std::string_view typeName, float speed = 2, float offset = 3);

    GMLIB_API bool throwEntity(Actor& projectile, float speed = 2, float offset = 3);

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

    GMLIB_NDAPI ItemStack& getMainHandSlot();

    GMLIB_API void setMainHandSlot(ItemStack& itemStack);

    GMLIB_NDAPI ItemStack& getOffHandSlot();

    GMLIB_API void setOffHandSlot(ItemStack& itemStack);

    GMLIB_API void
    hurtEntity(float damage, std::string_view causeName = "override", optional_ref<Actor> source = nullptr);

    GMLIB_NDAPI Biome& getBiome();

    GMLIB_API MCRESULT executeCommand(std::string_view command);
};