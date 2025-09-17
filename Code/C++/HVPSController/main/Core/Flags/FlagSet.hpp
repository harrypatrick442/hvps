#ifndef FLAG_SET_HPP
#define FLAG_SET_HPP

#include "FlagInternalHelper.hpp"
#include "FlagSnapshot.hpp"

// =======================================
// FlagSet wrapper
// =======================================
template <typename E>
class FlagSet {
    static_assert(std::is_enum_v<E>, "FlagSet requires an enum type");
    using Helper = flag_internal::FlagHelper<E>;
    using U = typename Helper::U;

public:
    FlagSet() : value(0) {}
    FlagSet(E flag) : value(Helper::toUnderlying(flag)) {}
    explicit FlagSet(U raw) : value(raw) {}

    bool has(E flag) const { return (value & Helper::toUnderlying(flag)) != 0; }
    void set(E flag) { value |= Helper::toUnderlying(flag); }
    void clear(E flag) { value &= ~Helper::toUnderlying(flag); }
    void toggle(E flag) { value ^= Helper::toUnderlying(flag); }

    // set + return snapshot of old state
    FlagSnapshot<E> setReturnOld(E flag) {
        U mask = Helper::toUnderlying(flag);
        U old = value;
        value |= mask;
        return FlagSnapshot<E>(old);
    }

    U raw() const { return value; }

private:
    U value;
};

#endif // FLAG_SET_HPP
