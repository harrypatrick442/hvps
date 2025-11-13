#include "./GetAvailableBluetoothDevicesRequest.hpp"
const char* GetAvailableBluetoothDevicesRequest::TYPE = "gabd";
GetAvailableBluetoothDevicesRequest::GetAvailableBluetoothDevicesRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t GetAvailableBluetoothDevicesRequest::getTicket(){
    return this->_ticket;
}
cJSON* GetAvailableBluetoothDevicesRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<GetAvailableBluetoothDevicesRequest> GetAvailableBluetoothDevicesRequest::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = std::make_shared<GetAvailableBluetoothDevicesRequest>(ticket);
return r;
}
GetAvailableBluetoothDevicesRequest::~GetAvailableBluetoothDevicesRequest(){
}
