#ifndef LASTABORTMESSAGE_HPP
#define LASTABORTMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
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
        bool _freeMemoryInDeconstructor;
   public:
        uint32_t* getBacktrace(size_t& length) noexcept;
        const char* getReason() noexcept;
        LastAbortMessage(
           uint32_t* backtrace,
           size_t backtraceLength, 
           const char* reason) noexcept;
        ~LastAbortMessage();
        static std::shared_ptr<LastAbortMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //LASTABORTMESSAGE_HPP
