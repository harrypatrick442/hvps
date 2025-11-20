#include "./GetAvailableBluetoothDevicesRequest.hpp"
const char* GetAvailableBluetoothDevicesRequest::TYPE = "gabd";
GetAvailableBluetoothDevicesRequest::GetAvailableBluetoothDevicesRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t GetAvailableBluetoothDevicesRequest::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* GetAvailableBluetoothDevicesRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
GetAvailableBluetoothDevicesRequest* GetAvailableBluetoothDevicesRequest::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new GetAvailableBluetoothDevicesRequest(ticket);
    cleanupBucket.addDelete(r);
    return r;
}
GetAvailableBluetoothDevicesRequest::~GetAvailableBluetoothDevicesRequest(){
}
