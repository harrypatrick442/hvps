#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <optional>

class Crc32 {
public:
    static constexpr uint32_t kPoly   = 0xEDB88320u;
    static constexpr uint32_t kInit   = 0xFFFFFFFFu;
    static constexpr uint32_t kXorOut = 0xFFFFFFFFu;

    // Runtime: compute CRC-32 for arbitrary bytes
    static uint32_t compute(const void* data, std::size_t len,
                            uint32_t seed = kInit) noexcept;

    // Runtime: compute CRC-32 of a trivially copyable POD/struct
    template <typename T>
    static uint32_t computePod(const T& pod, uint32_t seed = kInit) noexcept {
        static_assert(std::is_trivially_copyable_v<T>,
                      "computePod requires a trivially copyable type");
        const auto* p = reinterpret_cast<const unsigned char*>(&pod);
        return compute(p, sizeof(T), seed);
    }

private:
    static const std::array<uint32_t, 256>& getTable() noexcept;
    static std::array<uint32_t, 256> makeTable() noexcept;

    static inline std::optional<std::array<uint32_t, 256>> kTableOpt;
};
