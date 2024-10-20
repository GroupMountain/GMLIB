#pragma once
#include "Macros.h"
#include "mc/enums/ModalFormCancelReason.h"
#include "mc/world/actor/player/Player.h"

namespace gmlib::form {

using CustomFormElementResult = std::variant<int64, double, std::string, bool>;
using CustomFormResult        = std::unordered_map<uint, CustomFormElementResult>;

class CustomForm {
public:
    GMLIB_NDAPI explicit CustomForm(std::string const& title);

    GMLIB_NDAPI CustomForm();

public:
    GMLIB_API void setTitle(std::string const& title);

    GMLIB_API void onPlayerClose(std::function<void(Player&, ModalFormCancelReason)> callback);

    GMLIB_API uint appendLabel(std::string const& text);

    GMLIB_API uint appendInput(
        std::string const&                               text,
        std::string const&                               placeholder = {},
        std::string const&                               defaultVal  = {},
        std::function<void(Player&, std::string const&)> callback    = {}
    );

    GMLIB_API uint
    appendToggle(std::string const& text, bool defaultVal = false, std::function<void(Player&, bool)> callback = {});

    GMLIB_API uint appendDropdown(
        std::string const&                  text,
        std::vector<std::string> const&     options,
        size_t                              defaultVal = 0,
        std::function<void(Player&, int64)> callback   = {}
    );

    GMLIB_API uint appendSlider(
        std::string const&                   text,
        double                               min,
        double                               max,
        double                               step       = 0.0,
        double                               defaultVal = 0.0,
        std::function<void(Player&, double)> callback   = {}
    );

    GMLIB_API uint appendStepSlider(
        std::string const&                  text,
        std::vector<std::string> const&     steps,
        size_t                              defaultVal = 0,
        std::function<void(Player&, int64)> callback   = {}
    );

    GMLIB_API void sendTo(Player& player);

    GMLIB_API void sendTo(
        Player&                                                                               player,
        std::function<void(Player&, CustomFormResult&, std::optional<ModalFormCancelReason>)> callback
    );
};

} // namespace gmlib::form