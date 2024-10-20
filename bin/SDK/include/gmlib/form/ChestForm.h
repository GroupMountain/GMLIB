#pragma once
#include "Macros.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/item/registry/ItemStack.h"

namespace gmlib::form {

enum class ChestSlotType : int {
    Chest,
    Inventory,
    Cursor,
    // The SlotType::None will be showed in callback in some special circumstances,
    // such as player throwing the item out of the chest.
    // Do not use SlotType::None to register a slot !!!!!
    None
};
enum class ChestType : int { SingleChest, BigChest };
struct ChangingSlot {
    int           mSlot;
    ChestSlotType mType;
};

// src is the slot where player pick the item.
// dst is the slot where player place the item.
// amount is the amount of item changes.
using ChestUICallback = std::function<void(Player&, ChangingSlot const& src, ChangingSlot const& dst, int amount)>;

class ChestUI {
public:
    GMLIB_NDAPI explicit ChestUI(std::string const& name, ChestType formType = ChestType::BigChest);

    GMLIB_NDAPI ChestUI();

public:
    GMLIB_API ChestUI& setTitle(std::string const& name);

    GMLIB_API ChestUI& setType(ChestType formType);

    /**
     * Init a slot in the form.
     * @param slot If the type is SlotType::Cursor, this param won't take effect(The cursor has only one slot).
     * @attention A big chest's slot index is from 0 to 53. A chest's slot index is from 0 to 26, and a inventory's
     * slot index is from 0 to 35. If the slot param is out of the index, it won't take effect.
     */
    GMLIB_API ChestUI& registerSlot(int slot, ItemStack const& item, ChestSlotType type = ChestSlotType::Chest);

    // Hot change, return true if succeed
    GMLIB_API bool setSlot(int slot, ItemStack const& item, Player& pl, ChestSlotType type = ChestSlotType::Chest);

    GMLIB_API ChestUI& sendTo(Player& pl, ChestUICallback callback = {});

public:
    GMLIB_API static bool close(Player& pl);
};

class ChestForm {
public:
    GMLIB_API explicit ChestForm(std::string const& name, ChestType formType = ChestType::BigChest);

    GMLIB_API explicit ChestForm(
        std::string const& name,
        ItemStack const&   defaultFiller,
        ChestType          formType = ChestType::BigChest
    );

    GMLIB_NDAPI ChestForm();

public:
    GMLIB_API ChestForm& setTitle(std::string const& name);

    GMLIB_API ChestForm& setType(ChestType formType);

    GMLIB_API ChestForm& registerSlot(int slot, ItemStack const& item, std::function<void(Player&)> callback = {});

    GMLIB_API ChestForm& sendTo(Player& pl, bool autoClose = true);

public:
    GMLIB_API static bool close(Player& pl);
};

} // namespace gmlib::form