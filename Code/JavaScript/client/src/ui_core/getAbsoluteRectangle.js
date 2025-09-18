export default function getAbsoluteRectangle(element)
{
    var rect = element.getBoundingClientRect();
    var documentElement = document.documentElement;
    var t = rect.top + window.pageYOffset - documentElement.clientTop;
    var l = rect.left + window.pageXOffset - documentElement.clientLeft;
    return { t, l, w:rect.width, h:rect.height };    
}