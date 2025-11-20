#ifndef FILEINFO_HPP
#define FILEINFO_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class FileInfo
{
   private:
        const char* _name;
        int64_t _size;
        const char* _type;
   public:
        const char* getName()const noexcept;
        int64_t getSize()const noexcept;
        const char* getType()const noexcept;
        FileInfo(
           const char* name, 
           int64_t size, 
           const char* type) noexcept;
        ~FileInfo();
        static FileInfo* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //FILEINFO_HPP
