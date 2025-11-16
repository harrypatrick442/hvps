#ifndef FILEINFO_HPP
#define FILEINFO_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class FileInfo
{
   private:
        const char* _name;
        int64_t _size;
        const char* _type;
        bool _freeMemoryInDeconstructor;
   public:
        const char* getName() noexcept;
        int64_t getSize() noexcept;
        const char* getType() noexcept;
        FileInfo(
           const char* name, 
           int64_t size, 
           const char* type) noexcept;
        ~FileInfo();
        static std::shared_ptr<FileInfo> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //FILEINFO_HPP
