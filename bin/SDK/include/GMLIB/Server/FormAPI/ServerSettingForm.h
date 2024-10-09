#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/form/CustomForm.h"
#include "mc/world/actor/player/Player.h"

namespace GMLIB::Server::Form {

enum class IconType : uchar { Texture = 0, Url = 1 };

class ServerSettingForm {
public:
    GMLIB_API static int getDefaultPriority();

    GMLIB_API static bool hasTitle();

    GMLIB_API static std::string getTitle();

    GMLIB_API static bool setTitle(std::string const& title, bool forceModify = false);

    GMLIB_API static bool hasIcon();

    GMLIB_API static std::optional<std::string> getIconData();

    GMLIB_API static std::optional<IconType> getIconType();

    GMLIB_API static bool setIcon(std::string const& data, IconType type = IconType::Texture, bool forceModify = false);

    GMLIB_API static uint addLabel(
        std::string const&           text,
        std::function<bool(Player&)> playerDetector = [](Player& pl) -> bool { return true; },
        uint                         priority       = getDefaultPriority()
    );

    GMLIB_API static uint addInput(
        std::string const&                               text,
        std::string const&                               placeholder    = {},
        std::string const&                               defaultVal     = {},
        std::function<void(Player&, std::string const&)> callback       = nullptr,
        std::function<bool(Player&)>                     playerDetector = [](Player& pl) -> bool { return true; },
        uint                                             priority       = getDefaultPriority()
    );

    GMLIB_API static uint addToggle(
        std::string const&                 text,
        bool                               defaultVal     = false,
        std::function<void(Player&, bool)> callback       = nullptr,
        std::function<bool(Player&)>       playerDetector = [](Player& pl) -> bool { return true; },
        uint                               priority       = getDefaultPriority()
    );

    GMLIB_API static uint addDropdown(
        std::string const&                  text,
        std::vector<std::string> const&     options,
        size_t                              defaultVal     = 0,
        std::function<void(Player&, int64)> callback       = nullptr,
        std::function<bool(Player&)>        playerDetector = [](Player& pl) -> bool { return true; },
        uint                                priority       = getDefaultPriority()
    );

    GMLIB_API static uint addSlider(
        std::string const&                   text,
        double                               min,
        double                               max,
        double                               step           = 0.0,
        double                               defaultVal     = 0.0,
        std::function<void(Player&, double)> callback       = nullptr,
        std::function<bool(Player&)>         playerDetector = [](Player& pl) -> bool { return true; },
        uint                                 priority       = getDefaultPriority()
    );

    GMLIB_API static uint addStepSlider(
        std::string const&                  text,
        std::vector<std::string> const&     steps,
        size_t                              defaultVal     = 0,
        std::function<void(Player&, int64)> callback       = nullptr,
        std::function<bool(Player&)>        playerDetector = [](Player& pl) -> bool { return true; },
        uint                                priority       = getDefaultPriority()
    );

    GMLIB_API static bool removeElement(uint id);
};

} // namespace GMLIB::Server::Form