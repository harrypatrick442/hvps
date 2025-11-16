export default class GreetingRequest
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="gr";
    return r;
 }
}