#ifndef TESTMESSAGE_HPP
#define TESTMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class TestMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        TestMessage(
) noexcept;
        ~TestMessage();
        static std::shared_ptr<TestMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //TESTMESSAGE_HPP
