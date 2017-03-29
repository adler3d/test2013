// use as TGame method 
void make_screen_shoot(int iter)
{
  IDirect3DSurface9*surface;
  D3DDISPLAYMODE mode;
  auto*pDev=qDev.pDev;
  pDev->GetDisplayMode(0,&mode);
  pDev->CreateOffscreenPlainSurface(mode.Width,mode.Height,D3DFMT_A8R8G8B8,D3DPOOL_SCRATCH,&surface,NULL);
  if(pDev->GetFrontBufferData(0,surface)==D3D_OK)
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
      static HRESULT D3DXSaveSurfaceToFileA(
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
      typedef decltype(&t_hidden::D3DXSaveSurfaceToFileA) PGIL;
      //...
      string path;
      HMODULE Module;
      PGIL pProc;
      //...
      void DoReset()
      {
        path="d3dx9_24.dll";
        Module=0;
        pProc=0;
      }
      void init()
      {
        const string ProcName="D3DXSaveSurfaceToFileA";
        QapDebugMsg("step");
        Module=LoadLibrary(path.c_str());
        QapAssert(Module);
        pProc=(PGIL)GetProcAddress(Module,ProcName.c_str());
        QapAssert(pProc);
      }
      void free()
      {
        FreeLibrary(Module);
        pProc=nullptr;
      }
    };
    auto szFilename="screeenshot.png";
    DynamicContext dll;dll.init();
    auto D3DXSaveSurfaceToFile=dll.pProc;
    if(bool bWindowed=false) // a global config variable
    {
      RECT r;
      GetClientRect(hWnd,&r); // hWnd is our window handle
      POINT p = {0,0};
      ClientToScreen(hWnd,&p);
      SetRect(&r,p.x,p.y,p.x+r.right,p.y+r.bottom);
      D3DXSaveSurfaceToFile(szFilename,D3DXIFF_PNG,surface,NULL,&r);
    }
    else
      D3DXSaveSurfaceToFile(szFilename,D3DXIFF_PNG,surface,NULL,NULL);
  }
  surface->Release();
}
