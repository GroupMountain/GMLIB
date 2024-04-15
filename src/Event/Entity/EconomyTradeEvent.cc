#include "Global.h"
#include <GMLIB/Event/Entity/EconomyTradeEvent.h>
#include <mc/world/containers/ContainerScreenContext.h>
#include <mc/world/inventory/network/crafting/CraftHandlerTrade.h>
#include <mc/world/item/components/ItemStackNetResult.h>
#include <mc/world/item/crafting/MerchantRecipe.h>
#include <mc/world/item/trading/MerchantRecipeList.h>

namespace GMLIB::Event::EntityEvent {

Player& EconomyTradeBeforeEvent::getPlayer() const { return mPlayer; }
Player& EconomyTradeAfterEvent::getPlayer() const { return mPlayer; }

using MerchantRecipeTuple = std::tuple<::ItemStackNetResult, MerchantRecipe const*>;

LL_TYPE_INSTANCE_HOOK(
    EconomyTradeEventHook,
    HookPriority::Normal,
    CraftHandlerTrade,
    &CraftHandlerTrade::_getMerchantRecipeFromNetId,
    MerchantRecipeTuple,
    RecipeNetId const& tradeRecipeNetId
) {
    auto context = ll::memory::dAccess<ContainerScreenContext*>(this, 104);
    auto trader  = context->tryGetActor();
    if (trader) {
        auto& player      = context->getPlayer();
        auto  beforeEvent = EconomyTradeBeforeEvent(*trader, player);
        ll::event::EventBus::getInstance().publish(beforeEvent);
        if (beforeEvent.isCancelled()) {
            return {ItemStackNetResult::InvalidCraftResult, nullptr};
        }
        auto result     = origin(tradeRecipeNetId);
        auto afterEvent = EconomyTradeAfterEvent(*trader, player);
        ll::event::EventBus::getInstance().publish(afterEvent);
        return result;
    }
    return origin(tradeRecipeNetId);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class EconomyTradeBeforeEventEmitter : public ll::event::Emitter<emitterFactory1, EconomyTradeBeforeEvent> {
    ll::memory::HookRegistrar<EconomyTradeEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<EconomyTradeBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class EconomyTradeAfterEventEmitter : public ll::event::Emitter<emitterFactory2, EconomyTradeAfterEvent> {
    ll::memory::HookRegistrar<EconomyTradeEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<EconomyTradeAfterEventEmitter>();
}

} // namespace GMLIB::Event::EntityEvent