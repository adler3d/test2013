// for http://evilsquare.ru/ICG/
static bool IsKeyDown(int vKey)
{
  int i=GetAsyncKeyState(vKey);
  return i<0;
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
  auto hdc=GetDC(0);
  QapColor no_migrants=QapColor(0,20,56,70);auto na_myaso=QapColor(0,48,53,58);
  for(;!IsKeyDown(VK_LSHIFT);){if(IsKeyDown(VK_ESCAPE))return 0;Sleep(16);}
  auto getpix=[&](int x,int y){QapColor c=GetPixel(hdc,x,y);std::swap(c.r,c.b);return c;};
  auto click=[&](int x,int y){
    mouse_event(MOUSEEVENTF_LEFTDOWN,x,y,0,0);Sleep(16);
    mouse_event(MOUSEEVENTF_LEFTUP,x,y,0,0);Sleep(16);
  };
  bool grab_nub=false;POINT rp={1252,614};POINT kp={1308,564};
  while(!IsKeyDown(VK_ESCAPE)){
    auto r=IsKeyDown(VK_RSHIFT);
    auto L=IsKeyDown('L');
    auto k=IsKeyDown('K')||L;
    POINT arr[]={{1273,614},{1273,545},{1273,587},{1273,570},{1275,633}};
    if(IsKeyDown(VK_LSHIFT)||r||k){
      POINT P,B;auto&[x,y]=P;
      GetCursorPos(&P);
      if(r||k){
        click(x,y);click(x,y);
      }
      POINT np=rp;
      if(k){
        for(auto&p:arr)for(int dx=-1;dx<=1;dx++){auto c=getpix(p.x+dx*4,p.y);if(c==na_myaso){np=p;};}
      }
      B=P;if(r||k){P=np;SetCursorPos(x,y);}
      click(x,y);
      if(r||k){P=B;SetCursorPos(x,y);if(L)click(x,y);Sleep(k?100:16);}
      grab_nub=false;
    }else{
      auto G=IsKeyDown('G');
      auto N=IsKeyDown('N');
      if(G||N)
      {
        POINT t={286,305};
        auto check=[&](POINT p,QapColor c){auto g=getpix(p.x,p.y);
          #define F(R)abs(int(g.r-c.r))
          return (F(r)+F(g)+F(b))<9;
          #undef F
        };
        auto c=getpix(t.x,t.y);
        auto gotmig=no_migrants!=c;
        if(gotmig)Sleep(64);
        struct t_ohotnuk{QapColor c=QapColor(0,42,63,21);POINT p=POINT{272,273};};
        t_ohotnuk ohotnuk;
        t_ohotnuk bard={{0,155,123,87},{250,291}};
        t_ohotnuk dub={{0,255,255,195},{274,286}};
        t_ohotnuk sob={{0,63,95,29},{284,311}};
        if(gotmig)
        {
          //bool is_oh=check(ohotnuk.p,ohotnuk.c);
          //bool is_bd=check(bard.p,bard.c);
          //bool is_dub=check(dub.p,dub.c);
          t_ohotnuk sob={{0,63,95,29},{284,311}};
          if(check(sob.p,sob.c))//if(!is_oh&&!is_bd&&!is_dub)
          {
            grab_nub=true;
            POINT B;GetCursorPos(&B);
            SetCursorPos(t.x,t.y);
            click(t.x,t.y);
            SetCursorPos(rp.x,rp.y);
            click(rp.x,rp.y);
            SetCursorPos(B.x,B.y);
          }else{
            if(N){
              POINT B;GetCursorPos(&B);
              SetCursorPos(t.x,t.y);
              click(t.x,t.y);
              POINT p{1323,612};SetCursorPos(p.x,p.y);click(p.x,p.y);SetCursorPos(B.x,B.y);
            }
          }
        }
        {POINT t;GetCursorPos(&t);click(t.x,t.y);}
      }
      Sleep(16);
    }
    /*
    keybd_event(VK_W,0,0,0);
    Sleep(16);
    keybd_event(VK_W,0,KEYEVENTF_KEYUP,0);
    Sleep(16);
    */
  }
  return 0;
