#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>


// CRC-32 (IEEE 802.3) – polynomial 0xEDB88320, init/xorout 0xFFFFFFFF
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

    // Constexpr-friendly: compute for a std::array of bytes
    template <std::size_t N>
    static constexpr uint32_t compute(const std::array<uint8_t, N>& bytes,
                                      uint32_t seed = kInit) noexcept {
        uint32_t crc = seed;
        for (std::size_t i = 0; i < N; ++i) {
            const uint32_t idx = (crc ^ bytes[i]) & 0xFFu;
            crc = (crc >> 8) ^ kTable[idx];
        }
        return crc ^ kXorOut;
    }

private:
    // Build the 256-entry table at compile time
    static constexpr std::array<uint32_t, 256> makeTable() noexcept {
        std::array<uint32_t, 256> t{};
        for (uint32_t i = 0; i < 256; ++i) {
            uint32_t c = i;
            for (int k = 0; k < 8; ++k) {
                c = (c & 1u) ? (kPoly ^ (c >> 1)) : (c >> 1);
            }
            t[i] = c;
        }
        return t;
    }

    // The lookup table (inline so it’s a single ODR instance across TUs)
    static inline constexpr std::array<uint32_t, 256> kTable = makeTable();
};

