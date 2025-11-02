#pragma once
struct CrashRecord {
    uint32_t magic;
    esp_reset_reason_t reason;
    uint32_t pc;          // program counter
    uint32_t sp;          // stack pointer
    uint32_t excCause;    // exception cause if available
    uint32_t trace[8];    // up to 8 return addresses (optional)
    char message[128];
};
static_assert(sizeof(CrashRecord) % 4 == 0, "CrashRecord must be 4-byte aligned");