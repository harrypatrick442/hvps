#include "Crc.hpp"

uint32_t Crc32::compute(const void* data, std::size_t len, uint32_t seed) noexcept {
    const auto* p = static_cast<const unsigned char*>(data);
    uint32_t crc = seed;
    for (std::size_t i = 0; i < len; ++i) {
        const uint32_t idx = (crc ^ p[i]) & 0xFFu;
        crc = (crc >> 8) ^ kTable[idx];
    }
    return crc ^ kXorOut;
}
