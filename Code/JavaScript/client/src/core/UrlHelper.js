export default class UrlHelper {
    
    static getAbsoluteFromRelative(relativeUrl) {
        if (relativeUrl[0] !== '/')
            relativeUrl = '/' + relativeUrl;
        return  `${window.location.origin}${relativeUrl}`;
    }
    static seperateUrlIntoRootUrlAndFileName(url) {
        const lastIndexForwardSlash = url.lastIndexOf('/');
        if (lastIndexForwardSlash < 0) {
            return ['', url];
        }
        return [
            url.substring(0, lastIndexForwardSlash + 1),
            url.substring(lastIndexForwardSlash + 1, url.length)];
    }
    static makeSureSlashOnEnd(url) {
        if (url === null || url === undefined||url.length<1) return url;
        if (url[url.length - 1] === '/') return url;
        return url + '/';
    }
	static setSlashes(url, hasStartSlash, hasEndSlash){
        if (url === null || url === undefined||url.length<1) return url;
        if (url[url.length - 1] === '/') return url;
		const firstCharacter = url[0];
		const firstCharacterIsSlash=firstCharacter==='/';
		if(hasStartSlash){
			if(!firstCharacterIsSlash)
				url = '/'+url;
		}
		else{
			if (firstCharacterIsSlash)
				url = url.substring(1, url.length-1);
		}
		const lastCharacter = url[url.length-1];
		const lastCharacterIsSlash=lastCharacter==='/';
		if(hasEndSlash){
			if(!lastCharacterIsSlash)
				url = url+'/';
		}
		else{
			if(lastCharacterIsSlash)
				url = url.substring(0, url.length-1);
		}
		return url;
	}
    static removeStartSlash(url) {
        if (url === null || url === undefined||url.length<1) return url;
        if (url[0] === '/') return url.substring(1, url.length);
        return url;
    }
}