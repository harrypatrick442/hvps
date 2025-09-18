
function styleFromObject(element, style)
{
    for (var i in style)
    {
        var done = false;
        if (i.length > 2)
        {
            if (i.substring(0, 3) == '../')
            {
                var a = i.substring(3, i.length);
                element[a] = style[i];
                done = true;
            }
        }
        if (!done)
            element.style[i] = style[i];
    }
}
export default styleFromObject;