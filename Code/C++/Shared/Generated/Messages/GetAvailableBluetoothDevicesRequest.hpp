#ifndef GETAVAILABLEBLUETOOTHDEVICESREQUEST_HPP
#define GETAVAILABLEBLUETOOTHDEVICESREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class GetAvailableBluetoothDevicesRequest
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket()const noexcept;
        GetAvailableBluetoothDevicesRequest(
           uint64_t ticket = 0) noexcept;
        ~GetAvailableBluetoothDevicesRequest();
        static GetAvailableBluetoothDevicesRequest* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //GETAVAILABLEBLUETOOTHDEVICESREQUEST_HPP
