#pragma once
#include "GMLIB/Macros.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/effect/MobEffect.h"
#include "mc/world/item/registry/ItemStack.h"


namespace GMLIB::Server::Form {

class ChestForm {

public:
    enum SlotType : int {
        Chest,
        Inventory,
        Cursor,
        None
    }; // The SlotType::None will be showed in callback in some special circumstances, 
       // such as player throwing the item out of the chest.
        //Do not use SlotType::None to register a slot !!!!!

    enum FormType : int { SimpleChest, BigChest };
    struct ChangingSlot {
        int      slot;
        SlotType type;
    };

public:
    std::unordered_map<int, std::string> mChestItems     = std::unordered_map<int, std::string>();
    std::unordered_map<int, std::string> mInventoryItems = std::unordered_map<int, std::string>();
    std::string                          mCurorItem;
    std::string                          mName;
    FormType                             mFormType;
    std::function<void(ChestForm&, Player&, ChangingSlot, ChangingSlot, int)> mCallback;


public:
    /**
     * @param name: The title of the chest.
     * @param isBigChest: Whether the chest is a big chest.
     * @param mCallback: When player click any slot or close the chest, this will be triggered,and it will return -1
     * when player close it.
     */
    GMLIB_API ChestForm(
        std::string const& name,
        FormType           formType,

        // Src is the slot where player pick the item.
        // Dst is the slot where player place the item.
        // Amount is the amount of item changes.
        std::function<void(ChestForm&, Player&, ChangingSlot src, ChangingSlot dst, int amount)> mCallback
    );
    /**
     * Init a slot in the form.
     * @param slot If the type is SlotType::Cursor, this param won't take effect(The cursor has only one slot).
     * @attention A big chest's slot index is from 0 to 53. A chest's slot index is from 0 to 26, and a inventory's slot
     *            index is from 0 to 35. If the slot param is out of the index, it won't take effect.
     */
    GMLIB_API void registerSlot(int slot, ItemStack const& item, SlotType type);

    GMLIB_API bool
    setSlot(int slot, ItemStack const& item, Player& pl, SlotType type); // Hot change ,return true if success

    GMLIB_API void        sendTo(Player& pl);
    GMLIB_API static bool closeChestForm(Player& pl);

    ChestForm() = delete;
};


} // namespace GMLIB::Server::Form
