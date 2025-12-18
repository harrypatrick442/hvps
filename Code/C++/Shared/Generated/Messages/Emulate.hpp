#ifndef EMULATE_HPP
#define EMULATE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class Emulate
{
   public:
       static const char* TYPE;
   private:
   public:
        Emulate(
) noexcept;
        ~Emulate();
        static Emulate* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //EMULATE_HPP
