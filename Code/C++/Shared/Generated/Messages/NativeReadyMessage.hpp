#ifndef NATIVEREADYMESSAGE_HPP
#define NATIVEREADYMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class NativeReadyMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        NativeReadyMessage(
) noexcept;
        ~NativeReadyMessage();
        static std::shared_ptr<NativeReadyMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEREADYMESSAGE_HPP
