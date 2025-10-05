#ifndef NATIVESTORAGEDELETEALLRESPONSE_HPP
#define NATIVESTORAGEDELETEALLRESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeStorageDeleteAllResponse
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket() noexcept;
        NativeStorageDeleteAllResponse(
           uint64_t ticket) noexcept;
        ~NativeStorageDeleteAllResponse();
        static std::shared_ptr<NativeStorageDeleteAllResponse> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVESTORAGEDELETEALLRESPONSE_HPP
