function getAbsolute(element)
{
    var rect = element.getBoundingClientRect();
    var docEl = document.documentElement;
    var rectTop = rect.top + window.pageYOffset - docEl.clientTop;
    var rectLeft = rect.left + window.pageXOffset - docEl.clientLeft;
    var rectRight = rect.right + window.pageYOffset - docEl.clientTop;
    var rectBottom = rect.bottom + window.pageXOffset - docEl.clientLeft;
    return { top: rectTop, left: rectLeft, right:rectRight, bottom:rectBottom };    
}
export default getAbsolute;