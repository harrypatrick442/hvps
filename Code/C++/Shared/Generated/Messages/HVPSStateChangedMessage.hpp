#ifndef HVPSSTATECHANGEDMESSAGE_HPP
#define HVPSSTATECHANGEDMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class HVPSStateChangedMessage
{
   public:
       static const char* TYPE;
   private:
        int32_t _state;
   public:
        int32_t getState() noexcept;
        HVPSStateChangedMessage(
           int32_t state) noexcept;
        ~HVPSStateChangedMessage();
        static std::shared_ptr<HVPSStateChangedMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //HVPSSTATECHANGEDMESSAGE_HPP
