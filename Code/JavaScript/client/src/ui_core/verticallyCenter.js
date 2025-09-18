
function verticallyCenter(element)
{
    element.style.position = 'relative';
    element.style.top = '50%';
    element.style.transform = 'translateY(-50%)';
    element.style.msTransform = 'translateY(-50%)';
    element.style.webkitTransform = 'translateY(-50%)';
    element.style.oTransform = 'translateY(-50%)';
}
export default verticallyCenter;