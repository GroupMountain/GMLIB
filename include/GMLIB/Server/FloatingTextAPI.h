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

public:
    GMLIB_API FloatingText(std::string text, Vec3 position, DimensionType dimensionId);

    FloatingText() = delete;

public:
    GMLIB_API static FloatingText* getFloatingText(int64 runtimeId);

    GMLIB_API static bool deleteFloatingText(int64 runtimeId);

public:
    GMLIB_API virtual ~FloatingText();

public:
    GMLIB_API int64_t getFloatingTextRuntimeId();

    GMLIB_API void sendToClient(Player* pl);

    GMLIB_API void sendToAllClients();

    GMLIB_API void removeFromClient(Player* pl);

    GMLIB_API void removeFromAllClients();

    GMLIB_API void setText(std::string newText);

    GMLIB_API int64 getRuntimeID();

    GMLIB_API std::string getText();

    GMLIB_API Vec3 getPos();

    GMLIB_API DimensionType getDimensionId();
};

}