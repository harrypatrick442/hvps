export default class CoreDumpMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="cd";
    r["tn"]=o.taskName;
    r["tp"]=o.taskPointer;
    r["pc"]=o.programCounterForException;
    r["va"]=o.virtualAddressOfException;
    r["v"]=o.version;
    r["casha"]=o.crashingApplicationsSHA256SumAsAString;
    r["b"]=o.backtrace;
    r["coe"]=o.causeOfException;
    r["areg"]=o.aRegisterSetWhenTheExceptionCaused;
    r["pcreg"]=o.pCRegisterAddressAtExceptionLevel1To7;
    r["bitm"]=o.bitMaskOfAvailableEPCxRegisters;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.taskName=o["tn"];
    r.taskPointer=o["tp"];
    r.programCounterForException=o["pc"];
    r.virtualAddressOfException=o["va"];
    r.version=o["v"];
    r.crashingApplicationsSHA256SumAsAString=o["casha"];
    r.backtrace=o["b"];
    r.causeOfException=o["coe"];
    r.aRegisterSetWhenTheExceptionCaused=o["areg"];
    r.pCRegisterAddressAtExceptionLevel1To7=o["pcreg"];
    r.bitMaskOfAvailableEPCxRegisters=o["bitm"];
   return r;
 }
}