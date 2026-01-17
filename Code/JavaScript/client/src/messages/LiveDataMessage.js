export default class LiveDataMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="ld";
    r["a"]=o.outputVoltageVolts;
    r["b"]=o.outputVoltageValueBoundType;
    r["c"]=o.outputCurrentAmps;
    r["d"]=o.totalOutputEnergyJouls;
    r["e"]=o.firstStageVoltageVolts;
    r["f"]=o.firstStageVoltageValueBoundType;
    r["g"]=o.frequencyHz;
    r["h"]=o.frequencyHzValueBoundType;
    r["i"]=o.peakPrimaryCurrentAmps;
    r["l"]=o.peakPrimaryCurrentValueBoundType;
    r["j"]=o.primaryPowerWatts;
    r["k"]=o.totalPrimaryEnergyJouls;
    r["m"]=o.mosfetTemperatureDegreesC;
    r["n"]=o.lowerSnubberDiodeTemperatureDegreesC;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.outputVoltageVolts=o["a"];
    r.outputVoltageValueBoundType=o["b"];
    r.outputCurrentAmps=o["c"];
    r.totalOutputEnergyJouls=o["d"];
    r.firstStageVoltageVolts=o["e"];
    r.firstStageVoltageValueBoundType=o["f"];
    r.frequencyHz=o["g"];
    r.frequencyHzValueBoundType=o["h"];
    r.peakPrimaryCurrentAmps=o["i"];
    r.peakPrimaryCurrentValueBoundType=o["l"];
    r.primaryPowerWatts=o["j"];
    r.totalPrimaryEnergyJouls=o["k"];
    r.mosfetTemperatureDegreesC=o["m"];
    r.lowerSnubberDiodeTemperatureDegreesC=o["n"];
   return r;
 }
}