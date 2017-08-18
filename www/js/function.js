// 处理类名兼容性问题
function getClass(classname){
   if(document.getElementsByClassName){
     return document.getElementsByClassName(classname)
   }else{
     var tags=document.getElementsByTagName('*');
     var arr=[];
     for(var i=0;i<tags.length;i++){
     	if(tags[i].className==classname){
            arr.push(tags[i])
     	}
     }
     return arr;
   }
}
// 兼容的更改、获取、设置某个属性的值 
function cssrule(a,b){
  var a=a||0;
  var b=b||0;
  if(document.getElementsByClassName){
    return document.styleSheets[a].cssRules[b].style;
  }else{
    return document.styleSheets[a].rules[b].style;
  }
}
// 用来获得实际的样式属性
function getstyle(obj){
  if(document.getElementsByClassName){
    return getComputedStyle(obj,null)
  }else{
    return obj.currentStyle
  }
}

//通过节点获取所有的子元素
function getChilds(obj){
  var childs=obj.childNodes;
  var arr=[];
  for(var i=0;i<childs.length;i++){
    if(childs[i].nodeType==3&&/^\s+$/.test(childs[i].nodeValue)){
      continue;
    }else{
      arr.push(childs[i])
    }
  }
  return arr;
}

// 获得第一个子节点
function getFirst(obj){
   return getChilds(obj)[0]
}

// 获得最后一个子节点
function getLast(obj){
  return getChilds(obj)[getChilds(obj).length-1]
}

// 获得下一个兄弟节点的引用
function getNext(obj){
  var next=obj.nextSibling;
  if(next==null){
      return null;
     }
  while(next.nodeType!=1){
     next=next.nextSibling;
     if(next==null){
      return null;
     }
  } 
  return next;
}

// 获得上一个兄弟节点的引用
function getPrevious(obj){
  var previous=obj.previousSibling;
  if(previous==null){
      return null;
     }
  while(previous.nodeType!=1){
     previous=previous.previousSibling;
     if(previous==null){
      return null;
     }
  } 
  return previous;
}

//创建元素节点
function createObj(obj){
 return document.createElement(obj);
}

//将obj添加到父对象中
function appendObj(obj,parent){
   parent.appendChild(obj);
}
//将a添加到b的前面
function addBefore(a,b){
  var parent=b.parentNode;
   parent.insertBefore(a,b) 
}

//将a添加到b的后面
function addAfter(a,b){
  var parent=b.parentNode;
   var next=getNext(b);
   if(next==null){
    parent.appendChild(a)
   }else{
    parent.insertBefore(a,next) 
   }
}

//删除某个元素节点
function removeObj(obj){
  var parent=obj.parentNode;
  parent.removeChild(obj)
}

//将newobj替换oldobj
function repalceObj(newobj,oldobj){
  var parent=oldobj.parentNode;
  parent.replaceChild(newobj,oldobj);
}

//克隆obj(bull默认为false，代表只克隆obj本身；true代表克隆obj内部所有的东西)
function cloneObj(obj,bull){
   bull=false||bull;
   return obj.cloneNode(bull) 
}

// 1、添加事件处理程序(只能添加一个)
function addEvent(obj,event,fun){
  if(obj.addEventListener){
    obj.addEventListener(event,fun,false)
  }else{
    obj.attachEvent("on"+event,fun)
  }
}
// 2、删除事件处理程序(只能添加一个)
function removeEvent(obj,event,fun){
  if(obj.removeEventListener){
    obj.removeEventListener(event,fun,false) 
  }else{
   obj.dettachEvent("on"+event,fun) 
  }
}

//鼠标滚轮事件兼容性函数
function mousewheel(obj,fun1,fun2){
  if(obj.addEventListener){
    obj.addEventListener("mousewheel",scrollFn,false)//谷歌
    obj.addEventListener("DOMMouseScroll",scrollFn,false)//火狐
  }else{
    obj.attachEvent("onmousewheel",scrollFn)//IE
  }
  function scrollFn(e){
    var ev=e||window.event;
    if(ev.detail==3||ev.wheelDelta==-120){
      fun1.call(obj)
    }else if(ev.detail==-3||ev.wheelDelta==120){
      fun2.call(obj)
    }
  }
}


