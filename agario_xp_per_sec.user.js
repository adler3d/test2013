// ==UserScript==
// @name        xp/sec ( agar.io )
// @description Show xp per second stats after game
// @include     http://agar.io/*
// @include     http://www.agar.io/*
// @autor       Adler ( http://www.gamedev.ru/users/?id=31456 )
// @version     0.1 (30.07.2015)
// @grant       1
// ==/UserScript==

/*
window.localStorage.setItem("main","");
*/

function getDateTime() {
  var now     = new Date(); 
  var year    = now.getFullYear();
  var f=function(v){return (v.toString().length==1?'0':'')+v;}
  var month   = f(now.getMonth()+1); 
  var day     = f(now.getDate());
  var hour    = f(now.getHours());
  var minute  = f(now.getMinutes());
  var second  = f(now.getSeconds()); 
  var dateTime = year+'.'+month+'.'+day+' '+hour+':'+minute+':'+second;   
   return dateTime;
}
function getCookie(name){return window.localStorage.getItem(name);}
function setCookie(name,value){window.localStorage.setItem(name,value);}
function byclass(name){return document.getElementsByClassName(name);}
function get_stats()
{
  byclass("stats-food-eaten")[0].innerHTML
  byclass("stats-highest-mass")[0].innerHTML

  var arr="stats-food-eaten,stats-highest-mass,stats-time-alive,stats-leaderboard-time,stats-cells-eaten,stats-top-position".split(",");
  var obj={};
  for(var i=0;i<arr.length;i++)
  {
    var ex=arr[i];
    var span=byclass(ex)[0];
    obj[ex.split("stats-").join("")]=span.innerHTML;
  }
  return obj;
}
function time2sec(t){var ta=t.split(":");return (60*(+ta[0]))+(+ta[1]);;}
function xp_diff(cur,prev){
  var c=cur.xp.split(" XP").join("").split("/");
  var p=prev.xp.split(" XP").join("").split("/");
  if(p[1]!=c[1])return [false];
  return [true,c[0]-p[0]];
}
function g_doit()
{
  var bar=document.getElementsByClassName("progress-bar-text")[0].innerHTML;
  var lev=document.getElementsByClassName("progress-bar-star")[0].innerHTML;
  
  var c=getCookie("main")||"";
  if(c==""){var c={};c["n"]=-1;c["arr"]=[];}else{c=JSON.parse(c);}
  var prev="nothing";
  //prev=prev.split(",").join(",\n");
  c.n=+c.n+1;
  c.arr[c.n]={t:getDateTime(),xp:bar,lev:lev,stat:get_stats()};
  setCookie("main",JSON.stringify(c));
  if(c.n>=0)
  {
    var id=-1;
    for(var i=0;i<c.arr.length;i++){
      if(c.arr[i].stat["time-alive"]=="")continue;id=i;
    }
    if(id>=1)
    {
      var t=time2sec(c.arr[id].stat["time-alive"]);
      var prev=c.arr[id-1];
      var dxp=xp_diff(c.arr[id],c.arr[id-1]);
      if(dxp[0]){
        dxp=dxp[1];
        var prev=["time: "+prev.t,"xp: "+prev.xp,"lev: "+prev.lev,"id: "+id,"dxp: "+dxp,"t: "+t,"dxp/s: "+dxp/t].join("\n");
      }else{
        var prev=["time: "+prev.t,"xp: "+prev.xp,"lev: "+prev.lev];
      }
    }
  }
  var p=document.getElementById("g_pre");
  p.innerHTML="n: "+c.n+"\n"+prev;
};
function strip_promo(){
  var arr=document.getElementsByClassName("agario-party");
  for(var i=0;i<arr.length;i++){arr[i].parentNode.removeChild(arr[i]);}
  arr=document.getElementsByClassName("agario-promo");
  for(var i=0;i<arr.length;i++){arr[i].parentNode.removeChild(arr[i]);}
}
(function() {
  strip_promo();
  var p=document.getElementsByClassName("agario-profile-panel")[0];
  p.innerHTML+="<br><button class='btn' onclick='g_doit()'>update</button><br><br><pre id='g_pre'></pre>";
  var arr=[getDateTime,getCookie,setCookie,g_doit,get_stats,byclass,time2sec,xp_diff];var out=[];for(var i=0;i<arr.length;i++)out.push(arr[i].toSource());
  var code=out.join("\n");
  var s=document.createElement('script');
  s.innerHTML=code;
  document.body.appendChild(s);
})();
var g_z=window.localStorage.getItem("main");
g_z
var g_n=JSON.parse(g_z);
