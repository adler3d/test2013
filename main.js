http://habrahabr.ru/post/200190/
var arr=[1,2,5,4,5,1];
var map=[];
var max=arr[0];
for(var i=0;i<arr.length;i++){
  max=Math.max(max,arr[i]);
}
for(var i=0;i<arr.length;i++){
  var tmp=[];
  for(var j=0;j<max;j++){
    tmp[j]=arr[i]<j?'X':'O';
  }
  map[i]=tmp;
}
var out=[];
var count=0;
var test=function(x,y){
  var low=false;
  var top=false;
  for(var i=x-1;i>=0;i--){
    if(map[i][y]=='X')low=true;
  }
  for(var i=x+1;i<map.length;i++){
    if(map[i][y]=='X')top=true;
  }
  return low&&top;
}
for(var y=0;y<max;y++)
{
  var tmp=[];
  for(var x=0;x<arr.length;x++){
    tmp.push(map[x][y]);
    if(map[x][y]=='X')continue;
    if(!test(x,y))continue;
    count++;map[x][y]="w";
  }
  out[y]=tmp.join("");
}
return out.join("\n");