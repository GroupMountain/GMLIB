#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/LevelEvent.h"

namespace GMLIB::Event::LevelEvent {

class ExplodeBeforeEvent final : public ll::event::Cancellable<ll::event::world::LevelEvent> {
protected:
    BlockSource&        mBlockSource;
    optional_ref<Actor> mSource;
    Vec3&               mPosition;
    float&              mPower;
    bool&               mFire;
    bool&               mBreakBlocks;
    float&              mMaxResistance;
    bool&               mAllowUnderwater;

public:
    constexpr explicit ExplodeBeforeEvent(
        Level&              level,
        BlockSource&        blockSource,
        optional_ref<Actor> source,
        Vec3&               position,
        float&              power,
        bool&               fire,
        bool&               breakBlocks,
        float&              maxResistance,
        bool&               allowUnderwater
    )
    : Cancellable(level),
      mBlockSource(blockSource),
      mSource(source),
      mPosition(position),
      mPower(power),
      mFire(fire),
      mBreakBlocks(breakBlocks),
      mMaxResistance(maxResistance),
      mAllowUnderwater(allowUnderwater) {}

    GMLIB_NDAPI BlockSource& getBlockSource() const;
    GMLIB_NDAPI optional_ref<Actor> getSource() const;
    GMLIB_NDAPI Vec3&               getPosition() const;
    GMLIB_NDAPI float&              getPower() const;
    GMLIB_NDAPI bool&               canCauseFire() const;
    GMLIB_NDAPI bool&               canBreakBlocks() const;
    GMLIB_NDAPI float&              getMaxResistance() const;
    GMLIB_NDAPI bool&               canOverrideUnderwater() const;
};

class ExplodeAfterEvent final : public ll::event::world::LevelEvent {
protected:
    BlockSource&        mBlockSource;
    optional_ref<Actor> mSource;
    Vec3&               mPosition;
    float&              mPower;
    bool&               mFire;
    bool&               mBreakBlocks;
    float&              mMaxResistance;
    bool&               mAllowUnderwater;

public:
    constexpr explicit ExplodeAfterEvent(
        Level&              level,
        BlockSource&        blockSource,
        optional_ref<Actor> source,
        Vec3&               position,
        float&              power,
        bool&               fire,
        bool&               breakBlocks,
        float&              maxResistance,
        bool&               allowUnderwater
    )
    : LevelEvent(level),
      mBlockSource(blockSource),
      mSource(source),
      mPosition(position),
      mPower(power),
      mFire(fire),
      mBreakBlocks(breakBlocks),
      mMaxResistance(maxResistance),
      mAllowUnderwater(allowUnderwater) {}

    GMLIB_NDAPI BlockSource& getBlockSource() const;
    GMLIB_NDAPI optional_ref<Actor> getSource() const;
    GMLIB_NDAPI Vec3&               getPosition() const;
    GMLIB_NDAPI float&              getPower() const;
    GMLIB_NDAPI bool&               canCauseFire() const;
    GMLIB_NDAPI bool&               canBreakBlocks() const;
    GMLIB_NDAPI float&              getMaxResistance() const;
    GMLIB_NDAPI bool&               canOverrideUnderwater() const;
};

} // namespace GMLIB::Event::LevelEvent