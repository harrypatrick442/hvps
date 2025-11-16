#ifndef CONNECTTOBLUETOOTHDEVICEREQUEST_HPP
#define CONNECTTOBLUETOOTHDEVICEREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class ConnectToBluetoothDeviceRequest
{
   public:
       static const char* TYPE;
   private:
        const char* _address;
        uint64_t _ticket;
        bool _freeMemoryInDeconstructor;
   public:
        const char* getAddress() noexcept;
        uint64_t getTicket() noexcept;
        ConnectToBluetoothDeviceRequest(
           const char* address, 
           uint64_t ticket = 0) noexcept;
        ~ConnectToBluetoothDeviceRequest();
        static std::shared_ptr<ConnectToBluetoothDeviceRequest> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //CONNECTTOBLUETOOTHDEVICEREQUEST_HPP
