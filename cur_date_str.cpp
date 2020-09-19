//cls&&cl /Od /DEBUG /ZI cur_data_str.cpp /nologo /EHsc
//cls&&cl /O2 cur_data_str.cpp /nologo /EHsc
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
#include <vector>
#include <time.h>
#include <psapi.h>
#pragma comment(lib,"User32.lib")
using namespace std;

std::string GetLastErrorStdStr()
{
  DWORD error = GetLastError();
  if (error)
  {
    LPVOID lpMsgBuf;
    DWORD bufLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );
    if (bufLen)
    {
      LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
      std::string result(lpMsgStr, lpMsgStr+bufLen);
      
      LocalFree(lpMsgBuf);

      return result;
    }
  }
  return std::string();
}

static vector<string> split(const string&s,const string&needle){
  vector<string> arr;
  if(s.empty())return arr;
  size_t p=0;
  for(;;){
    auto pos=s.find(needle,p);
    if(pos==std::string::npos){arr.push_back(s.substr(p));return arr;}
    arr.push_back(s.substr(p,pos-p));
    p=pos+needle.size();
  }
  return arr;
}

static string local_cur_date_str_v3(){
  FILETIME ft;GetSystemTimePreciseAsFileTime(&ft);
  auto ts=ULARGE_INTEGER{ft.dwLowDateTime,ft.dwHighDateTime};
  auto us=(ts.QuadPart)%(10*1000*1000);
  SYSTEMTIME st;FileTimeToSystemTime(&ft,&st);
  auto lt=st;GetLocalTime(&lt);
  std::tm t={0};
  #define F(A,B,C)t.A=lt.B+C;
  F(tm_year,wYear,-1900);
  F(tm_mon,wMonth,-1);
  F(tm_wday,wDayOfWeek,0);
  F(tm_mday,wDay,0);
  F(tm_hour,wHour,0);
  F(tm_min,wMinute,0);
  F(tm_sec,wSecond,0);
  #undef F
  static const auto tm2str=[](const std::tm&t,int ms)->string{
    auto*ptm=&t;char buff[128];
    sprintf(&buff[0],"%02u:%02u:%02u.%03u %04u.%02u.%02u\0",
      ptm->tm_hour,ptm->tm_min,ptm->tm_sec,ms,
      1900+ptm->tm_year,ptm->tm_mon+1,ptm->tm_mday
    );
    return buff;
  };
  return tm2str(t,lt.wMilliseconds);
}
auto timestamp=local_cur_date_str_v3;

DWORD get_proc_id(HWND hwnd){DWORD dwProcId=0;GetWindowThreadProcessId(hwnd,&dwProcId);return dwProcId;}

string get_name_of_program_by_hwnd(string s,HWND hwnd){    
  auto dwProcId=get_proc_id(hwnd);
  char buffer[MAX_PATH]={0};
  HANDLE hProc=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,dwProcId);    
  auto rv=GetModuleFileNameExA((HMODULE)hProc,NULL,buffer,MAX_PATH);
  if(!rv){
    printf("error: %s\n",GetLastErrorStdStr().c_str());
  }
  CloseHandle(hProc);
  return buffer;
}

void inp(int vk,bool up){
  INPUT tmp;tmp.type=INPUT_KEYBOARD;auto&r=tmp.ki;
  r.wScan=0;r.time=0;r.dwExtraInfo=0;r.wVk=vk;
  r.dwFlags=up?KEYEVENTF_KEYUP:0;
  SendInput(1,&tmp,sizeof(tmp));
}

void kb_type_char(char c){
  auto u=[](auto k){
    inp(k,0);
    inp(k,1);
  };
  auto f=[c,u](char v,auto a,auto b){
    if(c!=v)return false;
    u(VK_SUBTRACT);
    return true;
  };
  if(f(':',5,8))return;
  if(f('.',4,6))return;
  auto k=VkKeyScan(c);
  u(k);
}

void kb_type(string s){for(auto&c:s)kb_type_char(c);}
static bool IsKeyDown(int vKey){int i=GetAsyncKeyState(vKey);return i<0;}

void on_hot_key(){ 
  auto s=timestamp();
  auto hwnd=GetForegroundWindow();
  string app_name=get_name_of_program_by_hwnd(s,hwnd);
  printf("[%s]: WM_HOTKEY received from app: %s\n",s.c_str(),app_name.c_str());
  if(split(app_name,"notepad++.exe").size()<=1){return;}

  keybd_event(VkKeyScan('T'),0x9e,KEYEVENTF_KEYUP,0);
  {auto K=VK_LMENU;if(IsKeyDown(K))keybd_event(K,0x9e,KEYEVENTF_KEYUP,0);}
  {auto K=VK_RMENU;if(IsKeyDown(K)){
    printf("look like keybd_event don't work with VK_RMENU :(\n");
    return;
  }}
  {auto K=VK_MENU;if(IsKeyDown(K))keybd_event(K,0x9e,KEYEVENTF_KEYUP,0);};
  kb_type("---\n"+s+"\n");
}

void winapi_set_hotkey_handler(...){
  size_t T='T';static const size_t B=0x42;static_assert('B'==B,"WTF?");
  size_t KEY=T;
  if(RegisterHotKey(NULL,1,MOD_ALT|MOD_NOREPEAT,KEY)){
    printf("[%s]: begin\n",timestamp().c_str());
  }else{
    printf("[%s]: RegisterHotKey failed - look like hotkey ALT+T already taken by other program.\n",timestamp().c_str());
    return;
  }
 
  MSG msg={0};
  while(GetMessage(&msg,NULL,0,0)!=0){
    if(msg.message==WM_HOTKEY){
      on_hot_key();
    }
  } 
}

int main(){
  printf("cur_date_str.exe - app that insert local datetime when you press ALT+T inside notepad++.exe\n");
  printf("[V5.0 build at 19:38:33.363 2020.09.19]\n");
  printf("   https://github.com/adler3d/test2013/blob/master/cur_date_str.cpp\n");
  winapi_set_hotkey_handler("ALT+T",on_hot_key);
  return 0;
}
