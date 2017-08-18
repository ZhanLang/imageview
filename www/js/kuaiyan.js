window.onload=function(){
	var lunbo=(function(){
		var lb=$('#lb');
		var num=0;
		var lis=lb.children('ul').eq(0).children('li');
		var liLenth=lis.length;
		var t;
		return function start(){
			function go(){
				num++
				if(num>=liLenth){
					num=0;
				}
				lis.attr('class','lis');
				lis.eq(num).attr('class','active');
			}
			t=setInterval(go,2000);
			lis.on('mouseenter',function(){
				clearInterval(t)
			});
			lis.on('mouseleave',function(){
				clearInterval(t);
				t=setInterval(go,2000);
			});
		}
	})()
	lunbo();
}