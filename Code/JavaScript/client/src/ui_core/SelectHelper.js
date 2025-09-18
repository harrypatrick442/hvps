var SelectHelper=new(function(){
var selectStrings =['webkitTouchCallout', 'webkitUserSelect', 'khtmlUserSelect', 'MozUserSelect', 'msUserSelect', 'userSelect', 'UserSelect'];
this.makeSelectable=function(element){
    setSelectable(element, "element");
};
this.makeUnselectable=function(element){
    setSelectable(element, "none");
};
function setSelectable(element, selectableString)
{
    if (element && element.type != 'text')//not text because this breaks internet explorer, stopping editing of text..
    {
        if (selectableString == "none")
        {
            if (element.nodeType == 1) {
                element.setAttribute("unselectable", "on");

            }
            if (element.onselectstart != undefined)// if IE
            {
                element.onselectstart = function () {
                    return false;
                };
            }
        }
        if (element.style)
        {
            var i = 0;
            selectStrings.forEach((selectString)=>{
                if (element.style[selectString] != undefined)// if Firefox
                {
                    element.style[selectString] = selectableString;
                }
                i++;
            });
        }
        var child = element.firstChild;
        while (child) {
            setSelectable(child, selectableString);
            child = child.nextSibling;
        }
    }
}
})();
export default SelectHelper;