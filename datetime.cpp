auto about_this_app="this is plugin for 'notepad++' that insert datetime inside text";
/*
echo this is build script
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvars32.bat"
cl /DUNICODE /D_UNICODE /O2 /LD datetime.cpp /nologo /EHsc
*/
#define NOMENUS
#define NOHELP
#define NOPROFILER
#define NOKANJI
#define NOWH
#define NOSERVICE
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
//#include <time.h>
#pragma comment(lib,"User32.lib")

#define NPPMSG  (WM_USER + 1000)
#define NPPM_GETCURRENTSCINTILLA  (NPPMSG + 4)
#define SCI_REPLACESEL 2170

struct SCNotification{};

const int nbChar = 64;

typedef const TCHAR * (__cdecl * PFUNCGETNAME)();

struct NppData{
  HWND _nppHandle;
  HWND _scintillaMainHandle;
  HWND _scintillaSecondHandle;
};

typedef void (__cdecl * PFUNCPLUGINCMD)();
        
struct ShortcutKey{
  bool _isCtrl;
  bool _isAlt;
  bool _isShift;
  UCHAR _key;
};
ShortcutKey ALT_T={false,true,false,'T'};

struct FuncItem{
  TCHAR _itemName[nbChar];
  PFUNCPLUGINCMD _pFunc;
  int _cmdID;
  bool _init2Check;
  ShortcutKey *_pShKey;
};

const int nbFunc = 2;
extern FuncItem funcItem[nbFunc];
extern NppData nppData;

FuncItem funcItem[nbFunc];
NppData nppData;

bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit){
  if(index>=nbFunc)return false;
  if(!pFunc)return false;
  lstrcpy(funcItem[index]._itemName, cmdName);
  funcItem[index]._pFunc = pFunc;
  funcItem[index]._init2Check = check0nInit;
  funcItem[index]._pShKey = sk;
  return true;
}

using std::string;
static string local_cur_date_str_v3(){
  FILETIME ft;GetSystemTimePreciseAsFileTime(&ft);
  SYSTEMTIME st;FileTimeToSystemTime(&ft,&st);
  auto lt=st;GetLocalTime(&lt);
  #define F(VAR,FIELD,OFFSET)auto VAR=lt.FIELD+OFFSET;
  F(Y,wYear,0);
  F(M,wMonth,0);
  F(D,wDay,0);
  F(h,wHour,0);
  F(m,wMinute,0);
  F(s,wSecond,0);
  F(ms,wMilliseconds,0);
  #undef F
  char buff[128];
  sprintf(&buff[0],"%02u:%02u:%02u.%03u %04u.%02u.%02u\0",
    h,m,s,ms,
    Y,M,D
  );
  return buff;
}

void insert_str_impl(const string&str){
  int which=-1;
  ::SendMessage(nppData._nppHandle,NPPM_GETCURRENTSCINTILLA,0,(LPARAM)&which);
  if(which==-1)return;
  HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

  ::SendMessage(curScintilla, SCI_REPLACESEL, 0, (LPARAM)str.c_str());
}

void insert_datetime(){
  auto s="---\n"+local_cur_date_str_v3()+"\n";
  insert_str_impl(s);
}

void insert_link_to_source_code(){
  auto s="---\n"+local_cur_date_str_v3()+"\n";
  s+="https://github.com/adler3d/test2013/blob/master/datetime.cpp";
  insert_str_impl(s);
}
    
void commandMenuInit(){
  setCommand(0, TEXT("insert_datetime"), insert_datetime, &ALT_T, false);
  setCommand(1, TEXT("insert_link_to_source_code"), insert_link_to_source_code, NULL, false);
}

const TCHAR NPP_PLUGIN_NAME[] = TEXT("datetime");

extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData){nppData=notpadPlusData;commandMenuInit();}
extern "C" __declspec(dllexport) const TCHAR * getName(){return NPP_PLUGIN_NAME;}    
extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF){*nbF=nbFunc;return funcItem;}
extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode){}
extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam){return TRUE;}
extern "C" __declspec(dllexport) BOOL isUnicode(){return TRUE;}
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  reasonForCall, LPVOID lpReserved){return TRUE;}
