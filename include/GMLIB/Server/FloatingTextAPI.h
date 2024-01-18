#pragma once
#include "GMLIB/GMLIB.h"
#include "mc/world/actor/player/Player.h"

class FloatingText {
public:
    std::string   mText;
    int64         mRuntimeId;
    Vec3          mPosition;
    DimensionType mDimensionId;

public:
    FloatingText(std::string text, Vec3 position, DimensionType dimensionId);
    FloatingText() = delete;

public:
    GMLIB_API static FloatingText* getFloatingText(int64 runtimeId);

    GMLIB_API static bool deleteFloatingText(int64 runtimeId);

public:
    GMLIB_API virtual ~FloatingText();

    GMLIB_API virtual int64_t getFloatingTextRuntimeId();

    GMLIB_API virtual void sendToClient(Player* pl);

    GMLIB_API virtual void sendToAllClients();

    GMLIB_API virtual void removeFromClient(Player* pl);

    GMLIB_API virtual void removeFromAllClients();

    GMLIB_API virtual void updateText(std::string newText);

    GMLIB_API virtual void removeFromServer();
};