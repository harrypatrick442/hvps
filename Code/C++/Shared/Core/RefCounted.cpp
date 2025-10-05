#include "RefCounted.hpp"

RefCounted::RefCounted() noexcept : _rc(1) {}

void RefCounted::addRef() const noexcept {
    _rc.fetch_add(1, std::memory_order_acq_rel);
}

void RefCounted::release() const noexcept {
	/*
    if (_rc.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete this;
    }*/
	
    const auto prev = _rc.fetch_sub(1, std::memory_order_acq_rel);
    // In debug builds, catch misuse early:
    assert(prev > 0 && "RefCount underflow: double release()");

    if (prev == 1) {
        // ensure all prior writes to *this* are visible before destruction
        std::atomic_thread_fence(std::memory_order_acquire);
        delete this;
    }
}
