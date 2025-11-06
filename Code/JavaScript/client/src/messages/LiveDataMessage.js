export default class LiveDataMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="ld";
    r["v"]=o.outputVoltage;
    r["c"]=o.outputCurrent;
    r["t"]=o.totalOutputEnergy;
    r["f"]=o.firstStageVoltage;
    r["p"]=o.peakPrimaryCurrent;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.outputVoltage=o["v"];
    r.outputCurrent=o["c"];
    r.totalOutputEnergy=o["t"];
    r.firstStageVoltage=o["f"];
    r.peakPrimaryCurrent=o["p"];
   return r;
 }
}