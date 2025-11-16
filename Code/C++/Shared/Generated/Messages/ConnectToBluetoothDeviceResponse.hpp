#ifndef CONNECTTOBLUETOOTHDEVICERESPONSE_HPP
#define CONNECTTOBLUETOOTHDEVICERESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
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
        bool _freeMemoryInDeconstructor;
   public:
        const char* getAddress() noexcept;
        std::optional<int32_t> getFailedReason() noexcept;
        uint64_t getTicket() noexcept;
        ConnectToBluetoothDeviceResponse(
           const char* address, 
           std::optional<int32_t> failedReason, 
           uint64_t ticket) noexcept;
        ~ConnectToBluetoothDeviceResponse();
        static std::shared_ptr<ConnectToBluetoothDeviceResponse> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //CONNECTTOBLUETOOTHDEVICERESPONSE_HPP
