#ifndef NATIVEGOTNEWTOKENMESSAGE_HPP
#define NATIVEGOTNEWTOKENMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class NativeGotNewTokenMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        NativeGotNewTokenMessage(
) noexcept;
        ~NativeGotNewTokenMessage();
        static std::shared_ptr<NativeGotNewTokenMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEGOTNEWTOKENMESSAGE_HPP
