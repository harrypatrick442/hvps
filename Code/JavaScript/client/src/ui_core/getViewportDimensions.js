function getViewportDimensions(){
	if(typeof document.documentElement !='undefined' &&
	typeof document.documentElement.clientWidth !='undefined' &&
	document.documentElement.clientWidth!=0)
		return [document.documentElement.clientWidth,document.documentElement.clientHeight];
	if(typeof window.innerWidth != 'undefined')
		return [window.innerWidth,window.innerHeight];
	var body = document.getElementsByTagName('body')[0];
	return [body.clientWidth, body.clientHeight];
}
export default getViewportDimensions;