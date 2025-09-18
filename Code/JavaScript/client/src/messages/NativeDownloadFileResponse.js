export default class NativeDownloadFileResponse
{
 static fromJSON(o){
    const r = {};
    r.directoryPath=o["fi"];
   return r;
 }
}