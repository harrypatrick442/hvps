#ifndef STARTEMULATE_HPP
#define STARTEMULATE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class StartEmulate
{
   public:
       static const char* TYPE;
   private:
   public:
        StartEmulate(
) noexcept;
        ~StartEmulate();
        static StartEmulate* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //STARTEMULATE_HPP
