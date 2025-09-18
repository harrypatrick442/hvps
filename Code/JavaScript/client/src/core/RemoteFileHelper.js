import {Promise} from "bluebird";
import ParseError from '../errors/ParseError';
import ParameterError from '../errors/ParameterError';
Promise.longStackTraces();
Promise.config({
    warnings: true,
    longStackTraces: true
});
export default class RemoteFileHelper {
    static loadJSON(url) {
		if(url===undefined||url===null)
		{
			throw new ParameterError(`url was ${url}`);
		}
        return new Promise((resolve, reject) => {
			const headers={"Content-Type": "application/json"};
            fetch(url, {
				method: 'GET',    
				credentials: "same-origin",
				headers:headers
			}).then((response) => {
                response.text().then((content) => {
                    resolve(JSON.parse(content));
                }).catch((err)=>{
					reject(new ParseError(`Parsing json from file path \"${url}\"`, err));
				});
            });
        })
    }
}