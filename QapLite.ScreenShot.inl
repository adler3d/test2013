void make_screen_shoot(const char*fn,IDirect3DSurface9*psrc=nullptr)
{
  IDirect3DSurface9*surface;
  auto*pDev=qDev.pDev;
  pDev->CreateOffscreenPlainSurface(consize.x,consize.y,D9Dev.PresParams.pp.BackBufferFormat,D3DPOOL_SYSTEMMEM,&surface,NULL);
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
