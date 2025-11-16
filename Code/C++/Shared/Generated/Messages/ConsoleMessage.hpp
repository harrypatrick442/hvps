#ifndef CONSOLEMESSAGE_HPP
#define CONSOLEMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class ConsoleMessage
{
   public:
       static const char* TYPE;
   private:
        bool _isError;
        const char* _message;
        bool _freeMemoryInDeconstructor;
   public:
        bool getIsError() noexcept;
        const char* getMessage() noexcept;
        ConsoleMessage(
           bool isError, 
           const char* message) noexcept;
        ~ConsoleMessage();
        static std::shared_ptr<ConsoleMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //CONSOLEMESSAGE_HPP
