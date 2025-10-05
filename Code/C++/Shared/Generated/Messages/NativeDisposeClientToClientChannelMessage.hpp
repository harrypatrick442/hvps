#ifndef NATIVEDISPOSECLIENTTOCLIENTCHANNELMESSAGE_HPP
#define NATIVEDISPOSECLIENTTOCLIENTCHANNELMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class NativeDisposeClientToClientChannelMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        NativeDisposeClientToClientChannelMessage(
) noexcept;
        ~NativeDisposeClientToClientChannelMessage();
        static std::shared_ptr<NativeDisposeClientToClientChannelMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEDISPOSECLIENTTOCLIENTCHANNELMESSAGE_HPP
