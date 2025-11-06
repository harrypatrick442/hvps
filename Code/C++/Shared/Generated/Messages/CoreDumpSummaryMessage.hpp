#ifndef COREDUMPSUMMARYMESSAGE_HPP
#define COREDUMPSUMMARYMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class CoreDumpSummaryMessage
{
   public:
       static const char* TYPE;
   private:
        uint32_t* _aRegisterSetWhenTheExceptionCaused;
        size_t _aRegisterSetWhenTheExceptionCausedLength;
        uint32_t* _backtrace;
        size_t _backtraceLength;
        bool _backtraceCorrupted;
        uint8_t _bitMaskOfAvailableEPCxRegisters;
        uint32_t _causeOfException;
        const char* _crashingApplicationsSHA256SumAsAString;
        uint32_t* _pCRegisterAddressAtExceptionLevel1To7;
        size_t _pCRegisterAddressAtExceptionLevel1To7Length;
        uint32_t _programCounterForException;
        const char* _taskName;
        uint32_t _taskPointer;
        uint32_t _version;
        uint32_t _virtualAddressOfException;
   public:
        uint32_t* getARegisterSetWhenTheExceptionCaused(size_t& length) noexcept;
        uint32_t* getBacktrace(size_t& length) noexcept;
        bool getBacktraceCorrupted() noexcept;
        uint8_t getBitMaskOfAvailableEPCxRegisters() noexcept;
        uint32_t getCauseOfException() noexcept;
        const char* getCrashingApplicationsSHA256SumAsAString() noexcept;
        uint32_t* getPCRegisterAddressAtExceptionLevel1To7(size_t& length) noexcept;
        uint32_t getProgramCounterForException() noexcept;
        const char* getTaskName() noexcept;
        uint32_t getTaskPointer() noexcept;
        uint32_t getVersion() noexcept;
        uint32_t getVirtualAddressOfException() noexcept;
        CoreDumpSummaryMessage(
           uint32_t* aRegisterSetWhenTheExceptionCaused,
           size_t aRegisterSetWhenTheExceptionCausedLength, 
           uint32_t* backtrace,
           size_t backtraceLength, 
           bool backtraceCorrupted, 
           uint8_t bitMaskOfAvailableEPCxRegisters, 
           uint32_t causeOfException, 
           const char* crashingApplicationsSHA256SumAsAString, 
           uint32_t* pCRegisterAddressAtExceptionLevel1To7,
           size_t pCRegisterAddressAtExceptionLevel1To7Length, 
           uint32_t programCounterForException, 
           const char* taskName, 
           uint32_t taskPointer, 
           uint32_t version, 
           uint32_t virtualAddressOfException) noexcept;
        ~CoreDumpSummaryMessage();
        static std::shared_ptr<CoreDumpSummaryMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //COREDUMPSUMMARYMESSAGE_HPP
