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

public:
    static FloatingText* getFloatingText(int64 runtimeId);

public:
    virtual ~FloatingText();

    virtual int64_t getFloatingTextRuntimeId();

    virtual void sendToClient(Player* pl);

    virtual void sendToAllClients();

    virtual void removeFromClient(Player* pl);

    virtual void removeFromAllClients();

    virtual void updateText(std::string newText);
};