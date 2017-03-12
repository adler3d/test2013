var arr=[0,1];
for(var id=1;id<5;id++){
  var n=Math.pow(2,id-1);var a0=0.5/n;var k=a0*2;// arr.push(a0+k*j)
  for(var j=0;j<n;j++){
    arr.push((1+j*2)+"/"+(n*2));
  }
}
return inspect(arr./*sort().map(e=>e*16).*/join(" "));
