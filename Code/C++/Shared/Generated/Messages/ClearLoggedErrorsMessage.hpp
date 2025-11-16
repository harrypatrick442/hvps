#ifndef CLEARLOGGEDERRORSMESSAGE_HPP
#define CLEARLOGGEDERRORSMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class ClearLoggedErrorsMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        ClearLoggedErrorsMessage(
) noexcept;
        ~ClearLoggedErrorsMessage();
        static std::shared_ptr<ClearLoggedErrorsMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //CLEARLOGGEDERRORSMESSAGE_HPP
