#ifndef COREDUMPSUMMARYMESSAGE_HPP
#define COREDUMPSUMMARYMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
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
        int32_t _subsystemIdentifier;
        const char* _taskName;
        uint32_t _taskPointer;
        uint32_t _version;
        uint32_t _virtualAddressOfException;
   public:
        uint32_t* getARegisterSetWhenTheExceptionCaused(size_t& length) noexcept;
        uint32_t* getBacktrace(size_t& length) noexcept;
        bool getBacktraceCorrupted()const noexcept;
        uint8_t getBitMaskOfAvailableEPCxRegisters()const noexcept;
        uint32_t getCauseOfException()const noexcept;
        const char* getCrashingApplicationsSHA256SumAsAString()const noexcept;
        uint32_t* getPCRegisterAddressAtExceptionLevel1To7(size_t& length) noexcept;
        uint32_t getProgramCounterForException()const noexcept;
        int32_t getSubsystemIdentifier()const noexcept;
        const char* getTaskName()const noexcept;
        uint32_t getTaskPointer()const noexcept;
        uint32_t getVersion()const noexcept;
        uint32_t getVirtualAddressOfException()const noexcept;
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
           int32_t subsystemIdentifier, 
           const char* taskName, 
           uint32_t taskPointer, 
           uint32_t version, 
           uint32_t virtualAddressOfException) noexcept;
        ~CoreDumpSummaryMessage();
        static CoreDumpSummaryMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //COREDUMPSUMMARYMESSAGE_HPP
