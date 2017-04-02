/*
  usage:
  void DoPreMove()
  {
    if(kb.OnUp(VK_F10))
    {
      DoSreenShot("rt.png");
    }
  }
*/
void FakeSceneRenderEx()
{
  auto&Dev=D9Dev;
  vec2i ClientSize=win.GetClientSize();
  if(!Dev.BeginScene())return;
  {
    D3DVIEWPORT9 ViewPort={0,0,ClientSize.x,ClientSize.y,-1.f,+1.f};
    Dev.pDev->SetViewport(&ViewPort);
    Dev.Set2D(vec2i(0,0),1.0,0,&ClientSize);
    Dev.Clear2D(0xffc8c8c8);
    qDev.NextFrame();
    SceneDoDraw();
  }
  if(!Dev.EndScene())return;
}
void DoSreenShot(const char*fn,bool use_rt=true)
{
  auto&pDev=qDev.pDev;
  IDirect3DSurface9*pout=nullptr;
  bool retval=pDev->CreateRenderTarget(consize.x,consize.y,D9Dev.PresParams.pp.BackBufferFormat,D3DMULTISAMPLE_NONE,0,0,&pout,0);
  if(retval!=D3D_OK)QapDebugMsg("retval="+IToS(retval));
  IDirect3DSurface9*pold=nullptr;
  pDev->GetRenderTarget(0,&pold);
  pDev->SetRenderTarget(0,pout);
  FakeSceneRenderEx();
  make_screen_shot(fn,pout);
  pDev->SetRenderTarget(0,pold);
  pold->Release();
  pold=nullptr;
  pout->Release();
  pout=nullptr;
}
void make_screen_shot(const char*fn,IDirect3DSurface9*psrc=nullptr)
{
  IDirect3DSurface9*surface;
  auto*pDev=qDev.pDev;
  if(psrc){
    pDev->CreateOffscreenPlainSurface(consize.x,consize.y,D9Dev.PresParams.pp.BackBufferFormat,D3DPOOL_SYSTEMMEM,&surface,NULL);
  }else{
    D3DDISPLAYMODE mode;
    pDev->GetDisplayMode(0,&mode);
    pDev->CreateOffscreenPlainSurface(mode.Width,mode.Height,D3DFMT_A8R8G8B8,D3DPOOL_SCRATCH,&surface,NULL);
  }
  auto retval=psrc?pDev->GetRenderTargetData(psrc,surface):pDev->GetFrontBufferData(0,surface);
  if(retval==D3D_OK)
  {
    enum D3DXIMAGE_FILEFORMAT{
      D3DXIFF_BMP         = 0,
      D3DXIFF_JPG         = 1,
      D3DXIFF_TGA         = 2,
      D3DXIFF_PNG         = 3,
      D3DXIFF_DDS         = 4,
      D3DXIFF_PPM         = 5,
      D3DXIFF_DIB         = 6,
      D3DXIFF_HDR         = 7,
      D3DXIFF_PFM         = 8,
      D3DXIFF_FORCE_DWORD = 0x7fffffff
    };
    struct t_hidden{
      static HRESULT WINAPI D3DXSaveSurfaceToFileA(
        _In_       LPCTSTR              pDestFile,
        _In_       D3DXIMAGE_FILEFORMAT DestFormat,
        _In_       LPDIRECT3DSURFACE9   pSrcSurface,
        _In_ const PALETTEENTRY         *pSrcPalette,
        _In_ const RECT                 *pSrcRect
      ){return 0;};
    };
    struct DynamicContext
    {
      DynamicContext(){DoReset();}
     ~DynamicContext(){if(pProc)free();}
      typedef decltype(&t_hidden::D3DXSaveSurfaceToFileA) PGIL;
      HMODULE Module;
      PGIL pProc;
      void DoReset(){Module=0;pProc=0;}
      void init(){Module=LoadLibrary("d3dx9_24.dll");pProc=(PGIL)GetProcAddress(Module,"D3DXSaveSurfaceToFileA");}
      void free(){FreeLibrary(Module);pProc=nullptr;}
    };
    DynamicContext dll;dll.init();
    auto D3DXSaveSurfaceToFile=dll.pProc;
    if(bool bWindowed=false)
    {
      RECT r;GetClientRect(hWnd,&r);
      POINT p={0,0};ClientToScreen(hWnd,&p);SetRect(&r,p.x,p.y,p.x+r.right,p.y+r.bottom);
      D3DXSaveSurfaceToFile(fn,D3DXIFF_PNG,surface,NULL,&r);
    }else
      D3DXSaveSurfaceToFile(fn,D3DXIFF_PNG,surface,NULL,NULL);
  }else QapDebugMsg(string(psrc?"GetRenderTargetData":"GetFrontBufferData")+" failed :(\n"+IToS(retval));
  surface->Release();
}
