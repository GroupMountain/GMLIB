#pragma once
#include "GMLIB/Macros.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/item/registry/ItemStack.h"

namespace GMLIB::Server::Form {

class ChestForm {
public:
    enum class SlotType : int {
        Chest,
        Inventory,
        Cursor,
        // The SlotType::None will be showed in callback in some special circumstances,
        // such as player throwing the item out of the chest.
        // Do not use SlotType::None to register a slot !!!!!
        None
    };

    enum class FormType : int { SimpleChest, BigChest };

    struct ChangingSlot {
        int      slot;
        SlotType type;
    };

protected:
    std::unordered_map<int, ItemStack>                                        mChestItems;
    std::unordered_map<int, ItemStack>                                        mInventoryItems;
    ItemStack                                                                 mCurorItem;
    std::string                                                               mName;
    FormType                                                                  mFormType;
    std::function<void(ChestForm&, Player&, ChangingSlot, ChangingSlot, int)> mCallback;

public:
    std::function<void(ChestForm&, Player&, ChangingSlot, ChangingSlot, int)> getCallback();

public:
    /**
     * @param name: The title of the chest.
     * @param isBigChest: Whether the chest is a big chest.
     * @param mCallback: When player click any slot or close the chest, this will be triggered,and it will return -1
     * when player close it.
     */
    GMLIB_API ChestForm(
        std::string const& name,
        // Src is the slot where player pick the item.
        // Dst is the slot where player place the item.
        // Amount is the amount of item changes.
        std::function<void(ChestForm&, Player&, ChangingSlot src, ChangingSlot dst, int amount)> callback,
        FormType formType = FormType::BigChest
    );

    ChestForm() = delete;

public:
    /**
     * Init a slot in the form.
     * @param slot If the type is SlotType::Cursor, this param won't take effect(The cursor has only one slot).
     * @attention A big chest's slot index is from 0 to 53. A chest's slot index is from 0 to 26, and a inventory's slot
     *            index is from 0 to 35. If the slot param is out of the index, it won't take effect.
     */
    GMLIB_API void registerSlot(int slot, ItemStack const& item, SlotType type);

    GMLIB_API bool
    setSlot(int slot, ItemStack const& item, Player& pl, SlotType type); // Hot change, return true if succeed

    GMLIB_API void sendTo(Player& pl);

public:
    GMLIB_API static bool closeChestForm(Player& pl);
};

} // namespace GMLIB::Server::Form
