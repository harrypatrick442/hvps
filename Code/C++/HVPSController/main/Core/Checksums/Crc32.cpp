#include "Crc32.hpp"

std::array<uint32_t, 256> Crc32::makeTable() noexcept {
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

const std::array<uint32_t, 256>& Crc32::getTable() noexcept {
    if (!kTableOpt.has_value()) {
        kTableOpt = makeTable();
    }
    return *kTableOpt;
}

uint32_t Crc32::compute(const void* data, std::size_t len, uint32_t seed) noexcept {
    const auto* p = static_cast<const unsigned char*>(data);
    uint32_t crc = seed;
    const auto& table = getTable();
    for (std::size_t i = 0; i < len; ++i) {
        const uint32_t idx = (crc ^ p[i]) & 0xFFu;
        crc = (crc >> 8) ^ table[idx];
    }
    return crc ^ kXorOut;
}
