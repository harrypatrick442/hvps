import LastAbortMessage from './LastAbortMessage';
import CoreDumpSummaryMessage from './CoreDumpSummaryMessage';
export default class GreetingResponse
{
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