import ParameterErrors from '../errors/ParameterErrors';
const {throwNotProvided }=ParameterErrors;
export default function _injectWitheldScripts() {
	const scripts = Array.from(document.getElementsByTagName('script'));
	scripts.forEach(script=>{
		try{
		const type=script.getAttribute('type');
		if(type!=='text/plain')return;
		const dataCookieConsent = script.getAttribute('data-cookie-consent');
		if(dataCookieConsent!=="tracking")return;
		script.parentNode.removeChild(script);
		script.type = 'text/javascript';
		document.body.appendChild(script);
		}
		catch(err){
			console.error(err);
		}
    });
}