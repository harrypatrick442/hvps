#ifndef ATOMIC_FLAG_SET_HPP
#define ATOMIC_FLAG_SET_HPP

#include <atomic>
#include "FlagInternalHelper.hpp"
#include "FlagSnapshot.hpp"

// ========================================================
// AtomicFlagSet wrapper
// ========================================================
template <typename E>
class AtomicFlagSet {
    static_assert(std::is_enum_v<E>, "AtomicFlagSet requires an enum type");
    using Helper = flag_internal::FlagHelper<E>;
    using U = typename Helper::U;

public:
    AtomicFlagSet() : value(0) {}
    AtomicFlagSet(E flag) : value(Helper::toUnderlying(flag)) {}
    explicit AtomicFlagSet(U raw) : value(raw) {}

    bool has(E flag, std::memory_order order = std::memory_order_acquire) const {
        return (value.load(order) & Helper::toUnderlying(flag)) != 0;
    }

    bool set(E flag, std::memory_order order = std::memory_order_acq_rel) {
        U mask = Helper::toUnderlying(flag);
        U old = value.fetch_or(mask, order);
        return (old & mask) != 0;
    }

    FlagSnapshot<E> setReturnOld(E flag, std::memory_order order = std::memory_order_acq_rel) {
        U mask = Helper::toUnderlying(flag);
        U old = value.fetch_or(mask, order);
        return FlagSnapshot<E>(old);
    }

    bool clear(E flag, std::memory_order order = std::memory_order_acq_rel) {
        U mask = Helper::toUnderlying(flag);
        U old = value.fetch_and(~mask, order);
        return (old & mask) != 0;
    }

    bool toggle(E flag, std::memory_order order = std::memory_order_acq_rel) {
        U mask = Helper::toUnderlying(flag);
        U old = value.fetch_xor(mask, order);
        return (old & mask) != 0;
    }

    void reset(std::memory_order order = std::memory_order_release) {
        value.store(0, order);
    }

    U raw(std::memory_order order = std::memory_order_acquire) const {
        return value.load(order);
    }

private:
    std::atomic<U> value;
};

#endif // ATOMIC_FLAG_SET_HPP
