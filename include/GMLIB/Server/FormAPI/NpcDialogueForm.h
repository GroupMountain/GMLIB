#pragma once
#include "GMLIB/GMLIB.h"


namespace GMLIB::Server::Form {

class NpcDialogueForm {
public:
    enum class NpcDialogueFormAction : int {
        Button = 0, // Button Mode
        Close  = 1, // Open Command
        Open   = 2  // Close Command
    };

public:
    std::string                                                                    mDialogue;
    std::string                                                                    mSceneName;
    std::string                                                                    mNpcName;
    nlohmann::ordered_json                                                         mActionJSON;
    uint64                                                                         mFormRuntimeId;
    std::function<void(Player* pl, int index, NpcRequestPacket::RequestType type)> mCallback;

public:
    GMLIB_API NpcDialogueForm(std::string npcName, std::string sceneName, std::string dialogue);

    NpcDialogueForm() = delete;

public:
    virtual ~NpcDialogueForm();

public:
    GMLIB_API int addAction(
        std::string              name,
        NpcDialogueFormAction    type     = NpcDialogueFormAction::Button,
        std::vector<std::string> commands = {}
    );

    GMLIB_API void
    sendTo(Player* pl, std::function<void(Player* pl, int index, NpcRequestPacket::RequestType type)> callback);
};

}