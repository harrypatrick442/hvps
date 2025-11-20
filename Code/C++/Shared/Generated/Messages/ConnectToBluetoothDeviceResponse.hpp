#ifndef CONNECTTOBLUETOOTHDEVICERESPONSE_HPP
#define CONNECTTOBLUETOOTHDEVICERESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class ConnectToBluetoothDeviceResponse
{
   public:
       static const char* TYPE;
   private:
        const char* _address;
        std::optional<int32_t> _failedReason;
        uint64_t _ticket;
   public:
        const char* getAddress()const noexcept;
        std::optional<int32_t> getFailedReason()const noexcept;
        uint64_t getTicket()const noexcept;
        ConnectToBluetoothDeviceResponse(
           const char* address, 
           std::optional<int32_t> failedReason, 
           uint64_t ticket) noexcept;
        ~ConnectToBluetoothDeviceResponse();
        static ConnectToBluetoothDeviceResponse* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //CONNECTTOBLUETOOTHDEVICERESPONSE_HPP
