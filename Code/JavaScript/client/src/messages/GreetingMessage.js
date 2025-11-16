import LastAbortMessage from './LastAbortMessage';
import CoreDumpSummaryMessage from './CoreDumpSummaryMessage';
export default class GreetingMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="g";
    const n = (v)=>v!==undefined&&v!==null;
    if(n(o.lastAbortMessage))
        r["la"]=LastAbortMessage.toJSON(o.lastAbortMessage);
    if(n(o.coreDumpSummaryMessage))
        r["cd"]=CoreDumpSummaryMessage.toJSON(o.coreDumpSummaryMessage);
    return r;
 }
 static fromJSON(o){
    const r = {};
    const n = (v)=>v!==undefined&&v!==null;
    let v;
    v = o["la"];
    if(n(v))
        r.lastAbortMessage=LastAbortMessage.fromJSON(v)
    v = o["cd"];
    if(n(v))
        r.coreDumpSummaryMessage=CoreDumpSummaryMessage.fromJSON(v)
   return r;
 }
}