
/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function Hover(element, callbackEnter, callbackLeave)
{
    var previousStyle;
    
    element.addEventListener('mouseenter', function(e){
        
                            if (!e) var e = window.event;
                            previousStyle=element.style.cssText; callbackEnter(e);
                        });
    element.addEventListener('mouseleave', function(e){
        
                            if (!e) var e = window.event;
                            element.style.cssText=previousStyle; if(callbackLeave){callbackLeave(e);}
                        });
}
function HoverAndClick(element, callbackEnter, callbackLeave, callbackMousedown, callbackMouseUp)
{
    var previousStyle;
    element.addEventListener('mouseenter', function(e){
        
                            if (!e) var e = window.event;
                            enter(e);
                        });
    element.addEventListener('mouseleave', function(e){
        
                            if (!e) var e = window.event;
                            leave(e);
                        });
    element.addEventListener('mousedown', function(e){
        
                            if (!e) var e = window.event;
                            if(callbackMousedown){leave(e);callbackMousedown(e);enter(e);}
                        });
    element.addEventListener('mouseup', function(e){
        
                            if (!e) var e = window.event;
                            if(callbackMouseUp){callbackMouseUp(e);}
                        });
    function enter(e)

    {
        previousStyle=element.style.cssText; if(callbackEnter)callbackEnter(e);
    }
    function leave(e)
    {
    element.style.cssText=previousStyle; if(callbackLeave){callbackLeave(e);}
    }
}
export default Hover;