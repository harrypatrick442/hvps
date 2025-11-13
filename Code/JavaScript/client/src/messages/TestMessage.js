export default class TestMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="test";
    return r;
 }
 static fromJSON(o){
    const r = {};
   return r;
 }
}