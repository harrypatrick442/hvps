#ifndef FLAG_INTERNAL_HELPER_HPP
#define FLAG_INTERNAL_HELPER_HPP

#include <cstdint>
#include <type_traits>

namespace flag_internal {

    // safe underlying conversion
    template <typename E>
    class FlagHelper {
        static_assert(std::is_enum_v<E>, "FlagHelper requires an enum type");
    public:
        using U = std::underlying_type_t<E>;

        static constexpr U toUnderlying(E e) noexcept {
            return static_cast<U>(e);
        }
    };

} // namespace flag_internal

#endif // FLAG_INTERNAL_HELPER_HPP
