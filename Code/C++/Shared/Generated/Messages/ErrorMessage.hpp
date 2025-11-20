#ifndef ERRORMESSAGE_HPP
#define ERRORMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
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
        int32_t getErrorType()const noexcept;
        const char* getSerializedError()const noexcept;
        ErrorMessage(
           int32_t errorType, 
           const char* serializedError) noexcept;
        ~ErrorMessage();
        static ErrorMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //ERRORMESSAGE_HPP
