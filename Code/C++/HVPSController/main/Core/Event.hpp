#pragma once
#include <cstddef>
#include <functional>
#include <vector>
#include <mutex>
#include <atomic>
#include <memory>
#include <algorithm>

// ---- Internal non-templated base so the handle can be non-templated
struct EventCoreBase {
    using HandlerId = std::size_t;
    virtual ~EventCoreBase() = default;
    virtual void remove(HandlerId id) = 0;
};

// ---- Public RAII handle (now bundled with Event.hpp)
class EventConnection {
public:
    using HandlerId = std::size_t;

    EventConnection() = default;
    EventConnection(std::weak_ptr<void> core, HandlerId id)
        : core_(std::move(core)), id_(id) {}

    EventConnection(const EventConnection&)            = delete;
    EventConnection& operator=(const EventConnection&) = delete;

    EventConnection(EventConnection&& other) noexcept { moveFrom(std::move(other)); }
    EventConnection& operator=(EventConnection&& other) noexcept {
        if (this != &other) { disconnect(); moveFrom(std::move(other)); }
        return *this;
    }

    ~EventConnection() noexcept{ disconnect(); }

    void disconnect() noexcept{
        if (id_ == 0) return;
        if (auto sp = core_.lock()) {
            auto* base = static_cast<EventCoreBase*>(sp.get());
            base->remove(id_);
        }
        id_ = 0;
        core_.reset();
    }

    bool connected() const noexcept{ return id_ != 0 && !core_.expired(); }
    HandlerId id()   const { return id_; }

private:
    void moveFrom(EventConnection&& o) noexcept {
        core_ = std::move(o.core_);
        id_   = o.id_;
        o.id_ = 0;
    }

    std::weak_ptr<void> core_;
    HandlerId id_{0};
};

// ---- Event template
template <typename... Args>
class Event {
public:
    using Handler   = std::function<void(Args...)>;
    using HandlerId = std::size_t;

    Event();
    ~Event() = default;

    EventConnection addHandler(Handler handler); // RAII handle
    void removeHandler(HandlerId id);            // optional manual removal
    void dispatch(Args... args);                 // fire
    std::size_t size() const;                    // handler count

private:
    struct HandlerEntry { HandlerId id; Handler func; };

    struct Core : EventCoreBase {
        std::mutex mtx;
        std::vector<HandlerEntry> handlers;
        std::atomic<HandlerId> nextId{1};

        void remove(HandlerId id) override {
            std::lock_guard<std::mutex> lock(mtx);
            auto it = std::remove_if(handlers.begin(), handlers.end(),
                                     [=](const HandlerEntry& e){ return e.id == id; });
            handlers.erase(it, handlers.end());
        }
    };

    std::shared_ptr<Core> core_;
};

#include "Event.tpp"  // keep at end so templates link up
