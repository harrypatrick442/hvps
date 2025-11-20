#ifndef TESTMESSAGE_HPP
#define TESTMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
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
        static TestMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //TESTMESSAGE_HPP
