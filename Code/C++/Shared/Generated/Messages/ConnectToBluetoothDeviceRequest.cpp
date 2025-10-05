#include "./ConnectToBluetoothDeviceRequest.hpp"
const char* ConnectToBluetoothDeviceRequest::TYPE = "ctbd";
ConnectToBluetoothDeviceRequest::ConnectToBluetoothDeviceRequest(
    const char* address, 
    uint64_t ticket):
        _address(address),
        _ticket(ticket){
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
    return std::make_shared<ConnectToBluetoothDeviceRequest>(address, ticket);
}
ConnectToBluetoothDeviceRequest::~ConnectToBluetoothDeviceRequest(){
}
