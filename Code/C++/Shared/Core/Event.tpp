#pragma once

template <typename... Args>
Event<Args...>::Event()
    : core_(std::make_shared<Core>()) {}

template <typename... Args>
EventConnection Event<Args...>::addHandler(Handler handler) {
    const HandlerId id = core_->nextId.fetch_add(1, std::memory_order_relaxed);
    {
        std::lock_guard<std::mutex> lock(core_->mtx);
        core_->handlers.push_back({id, std::move(handler)});
    }
    return EventConnection{std::weak_ptr<void>(core_), id};
}

template <typename... Args>
void Event<Args...>::removeHandler(HandlerId id) {
    core_->remove(id);
}

template <typename... Args>
void Event<Args...>::dispatch(Args... args) {
    // snapshot under lock
    std::vector<Handler> callList;
    {
        std::lock_guard<std::mutex> lock(core_->mtx);
        callList.reserve(core_->handlers.size());
        for (auto& e : core_->handlers) callList.push_back(e.func);
    }
    // invoke outside lock
    for (auto& fn : callList) fn(args...);
}

template <typename... Args>
std::size_t Event<Args...>::size() const {
    std::lock_guard<std::mutex> lock(core_->mtx);
    return core_->handlers.size();
}
