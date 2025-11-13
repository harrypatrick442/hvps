#include "./BluetoothDevice.hpp"
BluetoothDevice::BluetoothDevice(
    const char* address, 
    const char* name):
        _address(address),
        _name(name){
}
const char* BluetoothDevice::getAddress(){
    return this->_address;
}
const char* BluetoothDevice::getName(){
    return this->_name;
}
cJSON* BluetoothDevice::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "a", this->_address);
    JHelper::addString(j, "n", this->_name);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<BluetoothDevice> BluetoothDevice::fromJSON(cJSON* j){
    bool s = true;
    const char* address = JHelper::getString(j, "a", s);
    const char* name = JHelper::getString(j, "n", s);
    auto r = std::make_shared<BluetoothDevice>(address, name);
return r;
}
BluetoothDevice::~BluetoothDevice(){
}
