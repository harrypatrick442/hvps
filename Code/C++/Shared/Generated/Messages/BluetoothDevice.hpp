#ifndef BLUETOOTHDEVICE_HPP
#define BLUETOOTHDEVICE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class BluetoothDevice
{
   private:
        const char* _address;
        const char* _name;
   public:
        const char* getAddress()const noexcept;
        const char* getName()const noexcept;
        BluetoothDevice(
           const char* address, 
           const char* name) noexcept;
        ~BluetoothDevice();
        static BluetoothDevice* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //BLUETOOTHDEVICE_HPP
