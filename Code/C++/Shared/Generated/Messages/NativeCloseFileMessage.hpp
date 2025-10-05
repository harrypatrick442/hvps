#ifndef NATIVECLOSEFILEMESSAGE_HPP
#define NATIVECLOSEFILEMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class NativeCloseFileMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        NativeCloseFileMessage(
) noexcept;
        ~NativeCloseFileMessage();
        static std::shared_ptr<NativeCloseFileMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVECLOSEFILEMESSAGE_HPP
