#pragma once
#include "GMLIB/GMLIB.h"

class GMLIB_NpcDialogueForm {
private:
    std::string            mDialogue;
    std::string            mSceneName;
    std::string            mNpcName;
    nlohmann::ordered_json mActionJSON;

public:
    GMLIB_API GMLIB_NpcDialogueForm(std::string npcName, std::string sceneName, std::string dialogue);

    GMLIB_NpcDialogueForm() = delete;

public:
    virtual ~GMLIB_NpcDialogueForm() = default;

public:
    GMLIB_API void addButton(std::string name, std::vector<std::string> cmds);

    GMLIB_API void sendTo(Player* pl);
};