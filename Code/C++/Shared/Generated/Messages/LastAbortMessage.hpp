#ifndef LASTABORTMESSAGE_HPP
#define LASTABORTMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class LastAbortMessage
{
   public:
       static const char* TYPE;
   private:
        uint32_t* _backtrace;
        size_t _backtraceLength;
        const char* _reason;
        int32_t _subsystemIdentifier;
   public:
        uint32_t* getBacktrace(size_t& length) noexcept;
        const char* getReason()const noexcept;
        int32_t getSubsystemIdentifier()const noexcept;
        LastAbortMessage(
           uint32_t* backtrace,
           size_t backtraceLength, 
           const char* reason, 
           int32_t subsystemIdentifier) noexcept;
        ~LastAbortMessage();
        static LastAbortMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //LASTABORTMESSAGE_HPP
