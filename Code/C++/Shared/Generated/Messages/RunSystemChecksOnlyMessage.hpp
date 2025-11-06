#ifndef RUNSYSTEMCHECKSONLYMESSAGE_HPP
#define RUNSYSTEMCHECKSONLYMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class RunSystemChecksOnlyMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        RunSystemChecksOnlyMessage(
) noexcept;
        ~RunSystemChecksOnlyMessage();
        static std::shared_ptr<RunSystemChecksOnlyMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //RUNSYSTEMCHECKSONLYMESSAGE_HPP
