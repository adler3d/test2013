//    1 gold/с
//40 gold = 1.00 +gold/с; 1.00/40 = 0.025  (+gold/c)/gold
//50 gold = 1.70 +gold/c; 1.70/50 = 0.034  (+gold/c)/gold
function test(x,n)
{
  var sim=function(algo,x)
  {
    var gold=0;var income=1;
    for(var iter=0;iter<n;iter++){
      gold+=income;
      if(algo==1)
      {
        if(gold>=40)if(income==1){gold-=40;income+=1;}
        if(gold>=50)if(income==2){gold-=50;income+=x;}
      }
      if(algo==2)
      {
        if(gold>=50)if(income==1){gold-=50;income+=x;}
        if(gold>=40)if(income==1+x){gold-=40;income+=1;}
      }
    }
    return gold;
  }
  return [sim(1,x),sim(2,x)];
}
test(1.70,100);//[129.50000000000006,130.00000000000006]
