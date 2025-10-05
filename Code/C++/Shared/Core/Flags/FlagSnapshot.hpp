#ifndef FLAG_SNAPSHOT_HPP
#define FLAG_SNAPSHOT_HPP

#include <cstdint>
#include <type_traits>
#include "FlagInternalHelper.hpp"

// ========================================================
// FlagSnapshot: immutable snapshot of flags
// ========================================================
template <typename E>
class FlagSnapshot {
    static_assert(std::is_enum_v<E>, "FlagSnapshot requires an enum type");
    using Helper = flag_internal::FlagHelper<E>;
    using U = typename Helper::U;

public:
    explicit FlagSnapshot(U raw) : value(raw) {}

    // check if a flag was set
    bool has(E flag) const {
        return (value & Helper::toUnderlying(flag)) != 0;
    }

    // raw snapshot value
    U raw() const { return value; }

private:
    U value;
};

#endif // FLAG_SNAPSHOT_HPP
