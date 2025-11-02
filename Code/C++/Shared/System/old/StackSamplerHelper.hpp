#pragma once
#include "soc/soc_memory_layout.h"
class StackSamplerHelper {
public:
    static_assert(
    #if CONFIG_IDF_TARGET_ARCH_XTENSA
        true, "StackSamplerHelper: using Xtensa register mapping (EPC1/EXCCAUSE, SP=a1)"
    #elif CONFIG_IDF_TARGET_ARCH_RISCV
        true, "StackSamplerHelper: using RISC-V register mapping (mepc/mcause, SP=x2)"
    #else
        false, "StackSamplerHelper: unsupported architecture — add proper register mapping!"
    #endif
    );

    static inline __attribute__((always_inline)) void IRAM_ATTR storePcSpExec(uint32_t& out_pc, uint32_t& out_sp, uint32_t& out_exc)
    {
    #if CONFIG_IDF_TARGET_ARCH_XTENSA
        register uint32_t sp_reg __asm__("a1");
        out_sp = sp_reg;

        uint32_t epc1 = 0, exccause = 0;
        __asm__ volatile("rsr.epc1 %0"     : "=a"(epc1));
        __asm__ volatile("rsr.exccause %0" : "=a"(exccause));
        out_pc  = epc1;
        out_exc = exccause;

    #elif CONFIG_IDF_TARGET_ARCH_RISCV
        register uintptr_t sp_reg __asm__("sp");
        out_sp = (uint32_t)sp_reg;

        uint32_t mepc = 0, mcause = 0;
        __asm__ volatile("csrr %0, mepc"   : "=r"(mepc));
        __asm__ volatile("csrr %0, mcause" : "=r"(mcause));
        out_pc  = mepc;
        out_exc = mcause;

    #else
        register uintptr_t sp_reg __asm__("sp");
        out_sp = (uint32_t)sp_reg;
        out_pc = 0;
        out_exc = 0;
    #endif
    }

    static inline __attribute__((always_inline)) void IRAM_ATTR sampleStackWords(uint32_t sp_val, uint32_t* out, int n)
    {
        if (n <= 0) return;
        zeroU32(out, n);

        if ((sp_val & 0x3) != 0) return;
        if (!isInRamRange((uintptr_t)sp_val)) return;

        volatile uint32_t* sp = (volatile uint32_t*)sp_val;
        for (int i = 0; i < n; ++i) {
            uintptr_t addr = (uintptr_t)(sp + i);
            if (!isInRamRange(addr)) break;
            out[i] = sp[i];
        }
    }

private:
    static inline bool IRAM_ATTR isInRamRange(uintptr_t addr) {
        return ((addr >= (uintptr_t)SOC_DRAM_LOW && addr < (uintptr_t)SOC_DRAM_HIGH) ||
                (addr >= (uintptr_t)SOC_IRAM_LOW && addr < (uintptr_t)SOC_IRAM_HIGH));
    }

    static inline void IRAM_ATTR zeroU32(uint32_t* p, int n) {
        for (int i = 0; i < n; ++i) p[i] = 0;
    }
};
