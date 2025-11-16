#include "./ConnectToBluetoothDeviceRequest.hpp"
const char* ConnectToBluetoothDeviceRequest::TYPE = "ctbd";
ConnectToBluetoothDeviceRequest::ConnectToBluetoothDeviceRequest(
    const char* address, 
    uint64_t ticket):
        _address(address),
        _ticket(ticket),
        _freeMemoryInDeconstructor(false){
}
const char* ConnectToBluetoothDeviceRequest::getAddress(){
    return this->_address;
}
uint64_t ConnectToBluetoothDeviceRequest::getTicket(){
    return this->_ticket;
}
cJSON* ConnectToBluetoothDeviceRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "a", this->_address);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<ConnectToBluetoothDeviceRequest> ConnectToBluetoothDeviceRequest::fromJSON(cJSON* j){
    bool s = true;
    const char* address = JHelper::getString(j, "a", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = std::make_shared<ConnectToBluetoothDeviceRequest>(address, ticket);
r->_freeMemoryInDeconstructor = true;
return r;
}
ConnectToBluetoothDeviceRequest::~ConnectToBluetoothDeviceRequest(){
if(!_freeMemoryInDeconstructor)return;
     if(_address!=nullptr)delete[] _address;
}
