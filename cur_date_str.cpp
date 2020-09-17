//cls&&cl /Od /DEBUG /ZI cur_data_str.cpp /nologo /EHsc
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

static string conv_tm_to_str(const std::tm&t){
  auto*ptm=&t;
  char buff[128];
  sprintf(&buff[0],"%04u.%02u.%02u %02u:%02u:%02u\0",
    1900+ptm->tm_year,
    ptm->tm_mon+1,
    ptm->tm_mday,
    ptm->tm_hour,ptm->tm_min,ptm->tm_sec
  );
  return buff;
}

static string cur_date_str(){
  time_t rawtime;time(&rawtime);
  auto tm=*gmtime(&rawtime);
  return conv_tm_to_str(tm);
}

string get_name_of_program_by_hwnd(string s,HWND hwnd){    
  DWORD dwProcId=0;
  if(!GetWindowThreadProcessId(hwnd,&dwProcId))return NULL;
  char buffer[MAX_PATH]={0};
  HANDLE hProc=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,dwProcId);    
  auto rv=GetModuleFileNameExA((HMODULE)hProc,NULL,buffer,MAX_PATH);
  //printf("[%s]: GetModuleFileNameA(...)==%i\n",s.c_str(),rv);
  if(!rv){
    printf("error: %s\n",GetLastErrorStdStr().c_str());
  }
  CloseHandle(hProc);
  return buffer;
}

void kb_type_char(char c){
  if(c==':'){
    keybd_event(VK_LSHIFT,0x9e,0,0);
  }
  auto k=VkKeyScan(c);
  keybd_event(k,0x9e,0,0);
  keybd_event(k,0x9e,KEYEVENTF_KEYUP,0);
  if(c==':'){
    keybd_event(VK_LSHIFT,0x9e,KEYEVENTF_KEYUP,0);
  }
}

void kb_type(string s){for(auto&c:s)kb_type_char(c);}

void on_hot_key(){ 
  auto s=cur_date_str();
  string app_name=get_name_of_program_by_hwnd(s,GetForegroundWindow());
  printf("[%s]: WM_HOTKEY received from app: %s\n",s.c_str(),app_name.c_str());
  if(split(app_name,"notepad++.exe").size()<=1){return;}
  keybd_event(VkKeyScan('T'),0x9e,KEYEVENTF_KEYUP,0);
  keybd_event(VK_MENU,0x9e,KEYEVENTF_KEYUP,0);
  kb_type(s);
}

void winapi_set_hotkey_handler(...){
  size_t T='T';static const size_t B=0x42;static_assert('B'==B,"WTF?");
  size_t KEY=T;
  if(RegisterHotKey(NULL,1,MOD_ALT|MOD_NOREPEAT,KEY)){
    printf("[%s]: begin\n",cur_date_str().c_str());
  }
 
  MSG msg={0};
  while(GetMessage(&msg,NULL,0,0)!=0){
    if(msg.message==WM_HOTKEY){
      on_hot_key();
    }
  } 
}

int main(){
  winapi_set_hotkey_handler("ALT+T",on_hot_key);
  return 0;
}
/*
---
2020.09.17 11:14:56

done
*/
