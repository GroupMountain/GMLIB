#pragma once
#include "Macros.h"
#include "mc/network/packet/NpcRequestPacket.h"
#include "mc/world/actor/player/Player.h"
#include "nlohmann/json.hpp"

namespace gmlib::form {

class NpcDialogueForm {
public:
    enum class NpcDialogueFormAction : int {
        Button = 0, // Button Mode
        Close  = 1, // Open Command
        Open   = 2  // Close Command
    };

public:
    GMLIB_NDAPI explicit NpcDialogueForm(
        std::string const& npcName,
        std::string const& sceneName,
        std::string const& dialogue
    );

    GMLIB_NDAPI NpcDialogueForm();

public:
    GMLIB_API NpcDialogueForm& setNpcName(std::string const& name);

    GMLIB_API NpcDialogueForm& setSceneName(std::string const& name);

    GMLIB_API NpcDialogueForm& setDialogue(std::string const& dialogue);

    GMLIB_API NpcDialogueForm& addButton(std::string const& name, std::function<void(Player& pl)> callback = {});

    GMLIB_API NpcDialogueForm& onPlayerClose(std::function<void(Player& pl)> callback);

    GMLIB_API NpcDialogueForm& addAction(
        std::string const&              name,
        std::function<void(Player& pl)> callback = {},
        NpcDialogueFormAction           type     = NpcDialogueFormAction::Button,
        std::vector<std::string> const& commands = {}
    );

    GMLIB_API NpcDialogueForm& sendTo(Player& pl);
};

} // namespace gmlib::form