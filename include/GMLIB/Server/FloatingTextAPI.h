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
    ~FloatingText();

    FloatingText() = delete;

public:
    static FloatingText* getFloatingText(int64 runtimeId);

public:
    int64_t getFloatingTextRuntimeId();

    void sendToClient(Player* pl);

    void sendToAllClients();

    void removeFromClient(Player* pl);

    void removeFromAllClients();

    void updateText(std::string newText);
};