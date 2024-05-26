#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Listener.h"
#include "ll/api/schedule/Scheduler.h"
#include "ll/api/schedule/Task.h"
#include "mc/world/actor/player/Player.h"

namespace GMLIB::Server {

class FloatingTextBase {
protected:
    std::string   mText;
    int           mRuntimeId;
    Vec3          mPosition;
    DimensionType mDimensionId;
    bool          mTranslatePlaceholderApi;

public:
    GMLIB_NDAPI
    FloatingTextBase(
        std::string const& text,
        Vec3 const&        position,
        DimensionType      dimensionId,
        bool               translatePlaceholderApi = false
    );

public:
    GMLIB_API virtual ~FloatingTextBase();

    GMLIB_NDAPI virtual bool isDynamic() const;

    GMLIB_API virtual void sendTo(Player& pl);

    GMLIB_API virtual void sendToClients();

    GMLIB_API virtual void update(Player& pl);

    GMLIB_API virtual void updateClients();

    GMLIB_API virtual void removeFrom(Player& pl);

    GMLIB_API virtual void removeFromClients();

    GMLIB_NDAPI virtual int getRuntimeID() const;

    GMLIB_NDAPI virtual std::string getText() const;

    GMLIB_NDAPI virtual Vec3 getPosition() const;

    GMLIB_NDAPI virtual DimensionType getDimensionId() const;

    GMLIB_NDAPI virtual bool shouldTranslatePlaceholderApi() const;

    GMLIB_API virtual void setText(std::string const& newText);

    GMLIB_API virtual void setPosition(Vec3 const& pos);

    GMLIB_API virtual void setDimensionId(DimensionType dimId);

    GMLIB_API virtual void setTranslatePlaceholderApi(bool value);
};

class StaticFloatingText : public FloatingTextBase {
public:
    GMLIB_API
    StaticFloatingText(
        std::string const& text,
        Vec3 const&        position,
        DimensionType      dimensionId,
        bool               translatePlaceholderApi = false
    );

public:
    GMLIB_API void setText(std::string const& newText) override;

    GMLIB_API void setPosition(Vec3 const& pos) override;

    GMLIB_API void setDimensionId(DimensionType dimId) override;
};

class DynamicFloatingText : public StaticFloatingText {
private:
    uint                                                               mUpdateInterval;
    std::shared_ptr<ll::schedule::task::Task<ll::chrono::ServerClock>> mTask;

public:
    GMLIB_API
    DynamicFloatingText(
        std::string const& text,
        Vec3 const&        position,
        DimensionType      dimensionId,
        uint               seconds,
        bool               translatePlaceholderApi = true
    );

public:
    GMLIB_API ~DynamicFloatingText() override;

    GMLIB_NDAPI bool isDynamic() const override;

public:
    GMLIB_API bool stopUpdate();

    GMLIB_API bool startUpdate();

    GMLIB_API uint getUpdateInterval();

    GMLIB_API void setUpdateInterval(uint seconds);
};

class FloatingTextManager {
protected:
    std::vector<ll::event::ListenerId>                         mEventListener;
    std::unordered_map<int, std::shared_ptr<FloatingTextBase>> mRuntimeFloatingTexts;

public:
    FloatingTextManager();
    ~FloatingTextManager();

public:
    GMLIB_NDAPI static FloatingTextManager& getInstance();

    GMLIB_NDAPI static std::shared_ptr<StaticFloatingText> createStatic(
        std::string const& text,
        Vec3 const&        position,
        DimensionType      dimensionId,
        bool               translatePlaceholderApi
    );

    GMLIB_NDAPI static std::shared_ptr<DynamicFloatingText> createDynamic(
        std::string const& text,
        Vec3 const&        position,
        DimensionType      dimensionId,
        uint               updateInterval,
        bool               translatePlaceholderApi
    );

public:
    GMLIB_API bool add(std::shared_ptr<FloatingTextBase> floatingText);

    GMLIB_API bool remove(std::shared_ptr<FloatingTextBase> floatingText);

    GMLIB_API bool remove(int runtimeId);

    GMLIB_NDAPI optional_ref<FloatingTextBase> getFloatingText(int runtimeId);

    GMLIB_NDAPI std::vector<FloatingTextBase*> getAllFloatingTexts();

    GMLIB_NDAPI std::vector<FloatingTextBase*> getAllFloatingTexts(DimensionType dimId);
};


} // namespace GMLIB::Server