#pragma once
#include "GMLIB/GMLIB.h"
#include "mc/world/actor/player/Player.h"

namespace GMLIB::Server {

class FloatingText {
private:
    std::string   mText;
    int64         mRuntimeId;
    Vec3          mPosition;
    DimensionType mDimensionId;
    bool          mUsePapi;

public:
    GMLIB_API FloatingText(std::string text, Vec3 position, DimensionType dimensionId, bool usePapi = false);

    FloatingText() = delete;

public:
    GMLIB_API static FloatingText* getFloatingText(int64 runtimeId);

    GMLIB_API static bool deleteFloatingText(int64 runtimeId);

public:
    GMLIB_API virtual ~FloatingText();

    GMLIB_API virtual bool isDynamic() const;

public:
    GMLIB_API void sendToClient(Player* pl);

    GMLIB_API void sendToAllClients();

    GMLIB_API void removeFromClient(Player* pl);

    GMLIB_API void removeFromAllClients();

    GMLIB_API void setText(std::string newText);

    GMLIB_API void setPosition(Vec3& pos, DimensionType dimid);

    GMLIB_API void setUsePapi(bool value);

    GMLIB_API int64 getRuntimeID() const;

    GMLIB_API std::string getText() const;

    GMLIB_API Vec3 getPos() const;

    GMLIB_API DimensionType getDimensionId() const;

    GMLIB_API bool shouldUsePapi() const;
};

class StaticFloatingText : public FloatingText {
public:
    GMLIB_API StaticFloatingText(std::string text, Vec3 position, DimensionType dimensionId, bool usePapi = false);

    StaticFloatingText() = delete;

public:
    GMLIB_API void updateText(std::string newText);

    GMLIB_API void updatePosition(Vec3& pos, DimensionType dimid);
};

class DynamicFloatingText : public FloatingText {
private:
    uint                                                               mUpdateRate;
    std::shared_ptr<ll::schedule::task::Task<ll::chrono::ServerClock>> mTask;

public:
    GMLIB_API
    DynamicFloatingText(std::string text, Vec3 position, DimensionType dimensionId, uint seconds, bool usePapi = true);

    DynamicFloatingText() = delete;

public:
    GMLIB_API virtual ~DynamicFloatingText();

    GMLIB_API virtual bool isDynamic() const;

public:
    GMLIB_API void updateText(std::string newText);

    GMLIB_API void updatePosition(Vec3& pos, DimensionType dimid);

    GMLIB_API bool stopUpdate();

    GMLIB_API bool startUpdate();

    GMLIB_API uint getUpdateRate();

    GMLIB_API void setUpdateRate(uint seconds);
};

} // namespace GMLIB::Server