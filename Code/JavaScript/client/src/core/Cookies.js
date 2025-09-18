import ParameterErrors from '../errors/ParameterErrors';
const {throwNotProvided }=ParameterErrors;
export default class Cookies {
	static setCookie(cname, cvalue, exdays) {
		const date = new Date();
		date.setTime(date.getTime() + (exdays*24*60*60*1000));
		const expires = "expires="+ date.toUTCString();
		document.cookie = cname + "=" + cvalue + ";" + expires + ";path=/";
	}
}