export default class HVPSLiveDataMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="ld";
    r["ov"]=o.outputVoltage;
    r["oc"]=o.outputCurrent;
    r["toe"]=o.totalOutputEnergy;
    r["fsv"]=o.firstStageVoltage;
    r["pc"]=o.peakPrimaryCurrent;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.outputVoltage=o["ov"];
    r.outputCurrent=o["oc"];
    r.totalOutputEnergy=o["toe"];
    r.firstStageVoltage=o["fsv"];
    r.peakPrimaryCurrent=o["pc"];
   return r;
 }
}