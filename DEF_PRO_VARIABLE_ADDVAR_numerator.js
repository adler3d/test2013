var arr2str=out=>"[\n"+out.map(e=>JSON.stringify(e)).join(",\n")+"\n]";
var provar="#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\\\n";
var arr0=POST.data.split(provar);
var arr=arr0.slice(1);//arr.filter(e=>e.indexOf("ADDEND()")>=0);
var linefilter=c=>c.trim().length;
var valid=f=>f.split("\n").map(c=>linefilter(c)?"1":"").join("").length;
var bef="ADDBEG()\\";var aft="ADDEND()";
var get_body=z=>{var t=z.split(aft);var n=t[0].split(bef);return n.length<=1?"":n[1];}
 var get_bef=e=>    e.split(aft)[0].split(bef)[0]+bef;
 var get_aft=e=>aft+e.split(aft).slice(1).join(aft);
var conv=w=>!valid(w)?w:w.split("\n").map((y,i)=>{var k="ADDVAR(";var t=y.split(k);return t.length==1?y:t[0]+k+(i-1)+","+t.slice(1).join("k");}).join("\n");
arr=arr.map(x=>{
  var b=get_body(x);
  return !valid(b)?x:get_bef(x)+conv(b)+get_aft(x);
});
var toug_gen=(func,bef,aft)=>{
  var shit=s=>s.split(func).join("").split(bef).join(aft).split("));").join(");");
  var f=s=>s.split("\n").map(e=>e.indexOf(func)<0?e:shit(e)).join("\n");
  return f;
}
var need_slow_version=1;
var toug=s=>{
  if(!need_slow_version)return s;
  s=toug_gen("QapStrFinder::fromArr(")(s);
  s=toug_gen("CharMask::fromStr(","\");","\";")(s);
  return s;
}
return toug([arr0[0]].concat(arr).join(provar));// arr2str();//.filter(q=>valid(get_body(q))));//.length;//[0];
