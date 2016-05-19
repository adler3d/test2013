g_arr=POST['data'].split("\n```\n");
var t=g_arr[0].split("\n");
var m=g_arr[1].split("\n");
var out=[];var f=s=>"("+s+")";
//for(var i=0;i<t.length;i++)out.push(f(t[i]+";"+100*(i?(m[i]-m[i-1])/(t[i]-t[i-1])/m[i-1]:0)));
for(var i=0;i<t.length;i++)out.push(f(t[i]+";"+m[i]));
return out.join("\n");
//result:
//  http://yotx.ru/#!1/3_h/ubW/tbiH0jhvC/tn@0f7B/sO8npdb2NzYPIIgzHmhtc@tgd@OAdwEB7W4cQA5gFxcXFxcXFxcXFxcXFxcXPBDkYnfjALJ1enp6enp6enp6enp6enp6yjuDbO1uHEB2znhnENjuxgHk4uLi4uLi4uLi4uLi4uLigncKOdvdOICAtk5PT09PT09PT09PT08RPARka3fjAHLGQ0BOdzcOIGewi4uLi4uLi4uLi4uLCxAPBtnZ3TiAnJ6enp6enp6enp6enp6envJgkNPdjQMI4ox3Cdna3TiAwC4uLi4uLi4uLi4uLi4uQLxLyNnuxgHkcuv09PT09PT09PT09PT0lHcJge1ubPG2DnY3tiBnvK0tyNnuxg5vawdyuruxAznjbV1AELsbF7wtEAS2u3EBOeNtnUFOdzdAvK1TCGh3AwQ5420hIDu7G2e8LRjkYnfjDHLG27qEXOxunPJ2DiCg3Y1TyBlvZwtyuruB4O3sQC53N2C8nTPI2e7GJW8HBtna3dg64F1sQUC7G1tbvAsQ5HJ3Y2uHdwGDwHY3ti54oAsIaHdjC8QDwSCw3Y2tM94ZCALb3dg65Z1uQQ52N7YQvNNTyMXu/s4@iYbd2DllPJ5uMR63Li9297f29wEH
//source_img:
//  http://voproshaika.ru/wp-content/uploads/2013/05/Skolko-doljen-vesit-rebenok.jpg
//POST['data']:
/*
-0.75
0
0.08333333333333333
0.16666666666666666
0.25
0.3333333333333333
0.4166666666666667
0.5
0.5833333333333334
0.6666666666666666
0.75
0.8333333333333334
0.9166666666666666
1
1.5
2
2.5
3
3.5
4
4.5
5
5.5
6
6.5
7
8
9
10
11
12
13
14
15
16
17
```
4e-10
3.4
4.3
5.1
5.8
6.5
7.1
7.6
8.2
8.6
9.1
9.5
9.8
10
11.5
12.6
13.7
14.8
15.6
16.4
17.2
18.3
19.3
20.4
21.6
22.9
25.5
28.1
31.4
34.9
38.8
43.4
48.8
54.8
61.0
66.3
*/
