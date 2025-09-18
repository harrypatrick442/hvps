export default class NativePickFileResponse
{
 static fromJSON(o){
    const r = {};
    r.success=o["s"];
    r.fileIdentifier=o["fi"];
    r.fileName=o["fn"];
    r.fileSize=o["fs"];
    r.fileType=o["ft"];
   return r;
 }
}