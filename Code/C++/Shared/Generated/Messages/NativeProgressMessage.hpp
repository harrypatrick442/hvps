#ifndef NATIVEPROGRESSMESSAGE_HPP
#define NATIVEPROGRESSMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class NativeProgressMessage
{
   private:
   public:
        NativeProgressMessage(
) noexcept;
        ~NativeProgressMessage();
        static std::shared_ptr<NativeProgressMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEPROGRESSMESSAGE_HPP
