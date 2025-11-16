#ifndef BLUETOOTHDEVICE_HPP
#define BLUETOOTHDEVICE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class BluetoothDevice
{
   private:
        const char* _address;
        const char* _name;
        bool _freeMemoryInDeconstructor;
   public:
        const char* getAddress() noexcept;
        const char* getName() noexcept;
        BluetoothDevice(
           const char* address, 
           const char* name) noexcept;
        ~BluetoothDevice();
        static std::shared_ptr<BluetoothDevice> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //BLUETOOTHDEVICE_HPP
