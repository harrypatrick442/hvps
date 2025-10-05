#include "./GetVoltageRequest.hpp"
const char* GetVoltageRequest::TYPE = "gv";
GetVoltageRequest::GetVoltageRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t GetVoltageRequest::getTicket(){
    return this->_ticket;
}
cJSON* GetVoltageRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<GetVoltageRequest> GetVoltageRequest::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    return std::make_shared<GetVoltageRequest>(ticket);
}
GetVoltageRequest::~GetVoltageRequest(){
}
