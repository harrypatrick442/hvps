#ifndef ERRORMESSAGE_HPP
#define ERRORMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class ErrorMessage
{
   public:
       static const char* TYPE;
   private:
        int32_t _errorType;
        const char* _serializedError;
   public:
        int32_t getErrorType() noexcept;
        const char* getSerializedError() noexcept;
        ErrorMessage(
           int32_t errorType, 
           const char* serializedError) noexcept;
        ~ErrorMessage();
        static std::shared_ptr<ErrorMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //ERRORMESSAGE_HPP
