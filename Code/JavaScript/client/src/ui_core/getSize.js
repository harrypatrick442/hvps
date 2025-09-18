export default function getSize(element)
{
    var rect = element.getBoundingClientRect();
    return [rect.width, rect.height];    
}