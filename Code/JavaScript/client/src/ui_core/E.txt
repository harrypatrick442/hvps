export default class E{
    static div(className) {
        return E.c('div', className);
    }
	static textNode(text){
		return document.createTextNode(text);
	}
	static img(className){ 
		return E.c('img', className);
	}
	static canvas (className){
		return E.c('canvas', className);
	}
    static table  (className) {
        return E.c('table', className);
    }
    static tr  (className){
        return E.c('tr', className);
    }
    static td  (className){
        return E.c('td', className);
    }
    static th  (className){
        return E.c('th', className);
    }
    static h3  (className){
        return E.c('h3', className);
    }
    static h2  (className){
        return E.c('h2', className);
    }
    static h1  (className){
        return E.c('h1', className);
    }
    static ul  (className){
        return E.c('ul', className);
    }
    static li  (className){
        return E.c('li', className);
    }
	static video (className){
		return E.c('video', className);
	}
	static iframe (className){
		return E.c('iframe', className);
	}
	static checkbox (className){
		return E.i( 'checkbox', className);
	}
    static text  (className, placeholder){
        const e =  E.i('text', className);
		if(placeholder)
			e.placeholder = placeholder;
		return e;
    }
    static password  (className){
        return E.i('password', className);
    }
    static textarea  (className){
        return E.c('textarea', className);
    }
    static span  (className){
        return E.c('span', className);
    }
    static select  (className){
        return E.c('select', className);
    }
    static option  (className){
        return E.c('option', className);
    }
    static table  (className){
        return E.c('table', className);
    }
    static colgroup  (className){
        return E.c('colgroup', className);
    }
    static col  (className){
        return E.c('col', className);
    }
    static button  (className){
        return E.c('button', className);
    }
    static a  (className){
        return E.c('a', className);
    }
    static label  (className){
        return E.c('label', className);
    }
	static file (className){
		return E.i('file', className);
	}
    static i(type, className) {
        var i = E.c('input', className);
        i.type = type;
        return i;
    }
    static c(name, className) {
        var element =  document.createElement(name);
		if(!element.addEventListener)
			element.addEventListener = element.attachEvent;
		if(className)
			element.classList.add(className);
		return element;
    }
}