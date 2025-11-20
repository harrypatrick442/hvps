#include "./BluetoothDevice.hpp"
BluetoothDevice::BluetoothDevice(
    const char* address, 
    const char* name):
        _address(address),
        _name(name){
}
const char* BluetoothDevice::getAddress()const noexcept{
    return this->_address;
}
const char* BluetoothDevice::getName()const noexcept{
    return this->_name;
}
cJSON* BluetoothDevice::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "a", this->_address);
    JHelper::addString(j, "n", this->_name);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
BluetoothDevice* BluetoothDevice::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    const char* address = JHelper::getString(j, "a", s);
    cleanupBucket.addDeleteArray(address);
    const char* name = JHelper::getString(j, "n", s);
    cleanupBucket.addDeleteArray(name);
    auto r = new BluetoothDevice(address, name);
    cleanupBucket.addDelete(r);
    return r;
}
BluetoothDevice::~BluetoothDevice(){
}
