import UrlHelper from '../core/UrlHelper';
const urlParams = new URLSearchParams(window.location.search);
export default class UrlParameters{
	static get token(){
		var regExp = new RegExp('(?:filesrelay.com|localhost:3000|10.0.2.2:3000)\/d\/([a-zA-Z0-9]+)');
		const match  = regExp.exec(window.location.href);
		if(!match)return null;
		const value = match[1];
		if(value===undefined)return null;
		return value;
	}
	static get debug(){
		return urlParams.has('debug');
	}
}