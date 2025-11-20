#ifndef CONSOLEMESSAGE_HPP
#define CONSOLEMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class ConsoleMessage
{
   public:
       static const char* TYPE;
   private:
        bool _isError;
        const char* _message;
   public:
        bool getIsError()const noexcept;
        const char* getMessage()const noexcept;
        ConsoleMessage(
           bool isError, 
           const char* message) noexcept;
        ~ConsoleMessage();
        static ConsoleMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //CONSOLEMESSAGE_HPP
