#ifndef CONNECTTOBLUETOOTHDEVICEREQUEST_HPP
#define CONNECTTOBLUETOOTHDEVICEREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class ConnectToBluetoothDeviceRequest
{
   public:
       static const char* TYPE;
   private:
        const char* _address;
        uint64_t _ticket;
   public:
        const char* getAddress()const noexcept;
        uint64_t getTicket()const noexcept;
        ConnectToBluetoothDeviceRequest(
           const char* address, 
           uint64_t ticket = 0) noexcept;
        ~ConnectToBluetoothDeviceRequest();
        static ConnectToBluetoothDeviceRequest* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //CONNECTTOBLUETOOTHDEVICEREQUEST_HPP
