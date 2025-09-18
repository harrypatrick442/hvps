export default class NativeDownloadFileRequest
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="ndf";
    r["r"]=o.throughServerReceiveUrl;
    r["f"]=o.fileName;
    return r;
 }
}