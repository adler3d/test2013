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

auto timestamp=local_cur_date_str_v4;

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
  printf("[V5.0 build at 21:09:13.531 2020.09.19]\n");
  printf("   cur_date_str.exe - app that insert local datetime when you press ALT+T inside notepad++.exe\n");
  printf("   source code:\n");
  printf("      https://github.com/adler3d/test2013/blob/master/cur_date_str.cpp\n");
  printf("   also look at new version(a plugin for notepad++):\n");
  printf("      https://github.com/adler3d/test2013/blob/master/datetime.cpp");
  winapi_set_hotkey_handler("ALT+T",on_hot_key);
  return 0;
}
