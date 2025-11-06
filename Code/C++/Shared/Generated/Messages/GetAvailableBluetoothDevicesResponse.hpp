#ifndef GETAVAILABLEBLUETOOTHDEVICESRESPONSE_HPP
#define GETAVAILABLEBLUETOOTHDEVICESRESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class GetAvailableBluetoothDevicesResponse
{
   public:
       static const char* TYPE;
   private:
                std::optional<int32_t> _failedReason;
        uint64_t _ticket;
   public:
                std::optional<int32_t> getFailedReason() noexcept;
        uint64_t getTicket() noexcept;
        GetAvailableBluetoothDevicesResponse(
           , 
           std::optional<int32_t> failedReason, 
           uint64_t ticket) noexcept;
        ~GetAvailableBluetoothDevicesResponse();
        static std::shared_ptr<GetAvailableBluetoothDevicesResponse> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //GETAVAILABLEBLUETOOTHDEVICESRESPONSE_HPP
