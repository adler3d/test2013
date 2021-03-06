auto about_this_app="this is plugin for 'notepad++' that insert datetime inside text";
/*
echo this is build script
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvars32.bat"
cl /DUNICODE /D_UNICODE /O2 /LD datetime.cpp /nologo /EHsc
*/
//   set fo="C:\Users\Adler\Desktop\IndeedSnake2Day\notepad-plus-plus\PowerEditor\bin\plugins\datetime"
//   cls&&cl /DUNICODE /D_UNICODE /O2 /LD datetime.cpp /nologo /EHsc&&@copy /Y datetime.dll %fo%
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

#define NPPMSG (WM_USER + 1000)
#define NPPM_GETCURRENTSCINTILLA  (NPPMSG + 4)
#define SCI_REPLACESEL 2170

#define SCI_GETSELECTIONSTART 2143
#define SCI_GETSELECTIONEND 2145
#define SCI_GETSELTEXT 2161
#define SCI_GETCURRENTPOS 2008

#define	RUNCOMMAND_USER       (WM_USER + 3000)
#define NPPM_GETCURRENTLINE   (RUNCOMMAND_USER + CURRENT_LINE)
#define NPPM_GETCURRENTCOLUMN (RUNCOMMAND_USER + CURRENT_COLUMN)
#define CURRENT_LINE 8
#define CURRENT_COLUMN 9

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

const int nbFunc = 3;
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

FILETIME get_systime_percise_as_filetime(){FILETIME ft;GetSystemTimePreciseAsFileTime(&ft);return ft;}
SYSTEMTIME filetime_to_systime(const FILETIME&ft){SYSTEMTIME st;FileTimeToSystemTime(&ft,&st);return st;}
SYSTEMTIME get_localtime(const SYSTEMTIME&src){SYSTEMTIME out;SystemTimeToTzSpecificLocalTime(0,&src,&out);return out;}
SYSTEMTIME filetime_to_localtime(const FILETIME&ft){return get_localtime(filetime_to_systime(ft));}
SYSTEMTIME get_percise_localtime(){return filetime_to_localtime(get_systime_percise_as_filetime());}
string systime_to_str(const SYSTEMTIME&st){
  #define F(VAR,FIELD)auto&VAR=st.FIELD;
  F(Y,wYear);
  F(M,wMonth);
  F(D,wDay);
  F(h,wHour);
  F(m,wMinute);
  F(s,wSecond);
  F(x,wMilliseconds);
  #undef F
  char buff[]="YYYY.MM.DD hh:mm:ss.xxx";
  //           0123456789 123456789 12
  constexpr int q=10;constexpr int Z='0';
  #define F()buff[id--]=Z+v%q;v/=q;
  {int v=Y;int id=+3;F()F()F()F()}
  {int v=M;int id=+6;F()F()      }
  {int v=D;int id=+9;F()F()      }
  {int v=h;int id=12;F()F()      }
  {int v=m;int id=15;F()F()      }
  {int v=s;int id=18;F()F()      }
  {int v=x;int id=22;F()F()F()   }
  #undef F
  return buff;
}
string filetime_to_localstr(const FILETIME&ft){return systime_to_str(filetime_to_localtime(ft));}
string local_cur_date_str_v4(){auto lt=get_percise_localtime();return systime_to_str(lt);}

/*
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
}*/

HWND getCurrentScintillaHandle(){
  int out=-1;
  ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&out);
  return !out?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
};

void insert_str_impl(const string&str){
  auto curScintilla=getCurrentScintillaHandle();
  ::SendMessage(curScintilla,SCI_REPLACESEL,0,(LPARAM)str.c_str());
}

string get_selected_text(){
  auto curScintilla=getCurrentScintillaHandle();
  auto start=::SendMessage(curScintilla,SCI_GETSELECTIONSTART,0,0);
  auto end=::SendMessage(curScintilla,SCI_GETSELECTIONEND,0,0);
  if(end<start)std::swap(start,end);

  auto n=end-start;
  if(!n)return "";

  string s;s.resize(n+1);auto*p=&s[0];
  ::SendMessage(curScintilla,SCI_GETSELTEXT,0,(LPARAM)p);
  return s;
}

struct t_curpos{int x=0;int y=0;int pos=0;};
t_curpos curpos(){
  auto curScintilla=getCurrentScintillaHandle();
  t_curpos out;
  out.pos=::SendMessage(curScintilla,SCI_GETCURRENTPOS,0,0);
  out.y=::SendMessage(nppData._nppHandle,NPPM_GETCURRENTLINE,0,0);
  out.x=::SendMessage(nppData._nppHandle,NPPM_GETCURRENTCOLUMN,0,0);
  return out;
}

void insert_datetime(){
  auto p=curpos();
  string br="\n";string es="";
  auto s=(p.x==0?es:br)+"---\n"+local_cur_date_str_v4()+"\n";
  insert_str_impl(s);
}

void insert_just_datetime(){insert_str_impl(local_cur_date_str_v4());}

void insert_link_to_source_code(){
  auto s="---\n"+local_cur_date_str_v4()+"\n";
  s+="https://github.com/adler3d/test2013/blob/master/datetime.cpp";
  insert_str_impl(s);
}
    
void commandMenuInit(){
  setCommand(0, TEXT("insert_datetime"), insert_datetime, &ALT_T, false);
  setCommand(1, TEXT("insert_just_datetime"), insert_just_datetime, NULL, false);
  setCommand(2, TEXT("insert_link_to_source_code"), insert_link_to_source_code, NULL, false);
}

const TCHAR NPP_PLUGIN_NAME[] = TEXT("datetime");

extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData){nppData=notpadPlusData;commandMenuInit();}
extern "C" __declspec(dllexport) const TCHAR * getName(){return NPP_PLUGIN_NAME;}    
extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF){*nbF=nbFunc;return funcItem;}
extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode){}
extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam){return TRUE;}
extern "C" __declspec(dllexport) BOOL isUnicode(){return TRUE;}
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  reasonForCall, LPVOID lpReserved){return TRUE;}
