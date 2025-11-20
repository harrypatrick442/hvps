#ifndef STATECHANGEDMESSAGE_HPP
#define STATECHANGEDMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class StateChangedMessage
{
   public:
       static const char* TYPE;
   private:
        int32_t _state;
   public:
        int32_t getState()const noexcept;
        StateChangedMessage(
           int32_t state) noexcept;
        ~StateChangedMessage();
        static StateChangedMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //STATECHANGEDMESSAGE_HPP
