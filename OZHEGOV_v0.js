function to_map(arr){var out={};for(var i=0;i<arr.length;i++)out[arr[i]]=1;return out;}
var allows=to_map(gen_dips_rus()+gen_dips("AZaz"));
var arr=POST['data'].split("\n");
var out=[];
var map={};var map2arr={};
var seps=to_map("0123456789 !,+?-.()=:[]<>/\"'¦¬\;*`&_%{}ў");
//function f(s){for(var i=0;i<s.length;i++)if(!(s[i] in allows))seps[s[i]]=1;};
function reg(from_raw,who_raw){var from=from_raw.toLowerCase();var who=who_raw.toLowerCase();if(from==who)return;if(!(who in map2arr))map2arr[who]={};map2arr[who][from]=1;};
function reg_words_from(from,s){
  var tmp="";
  for(var i=0;i<s.length;i++)
  {
    if(!(s[i] in allows))
    {
      if(tmp.length>0){reg(from,tmp);tmp="";};
    }else tmp+=s[i];
  }
  if(tmp.length>0){reg(from,tmp);tmp="";};
}
for(var i=0;i<arr.length;i++){
  var ex=arr[i];
  var a=ex.split("|");
  if(a[0].indexOf("-")>=0)continue;
  if(a[0].indexOf(".")>=0)continue;
  if(a[0].indexOf(")")>=0)continue;
  for(var j=1;j<a.length;j++)reg_words_from(a[0],a[j]);
  out.push(ex);
  if(!(a[0] in map))map[a[0]]=ex;
}
//return map["программа"];
var wmt20={};
for(var k in map2arr){if(Object.keys(map2arr[k]).length!=5)continue;if(!(k in map))continue;wmt20[k]=Object.keys(map2arr[k]);}
return JSON.stringify(wmt20,null,"  ");Object.keys(wmt20).length;out.length;out.length;
