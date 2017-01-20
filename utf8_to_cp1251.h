static string utf8_to_cp1251(const string&s){
  auto n=s.size();
  if(s.empty())return "";
  auto*utf8=&s[0];
  vector<wchar_t> tmp(n,0);auto*wstr=&tmp[0];
  n=MultiByteToWideChar(CP_UTF8,0,utf8,n,wstr,n);
  if(!n)return 0;
  string out(n,0);auto*s1251=&out[0];
  WideCharToMultiByte(1251,0,wstr,n,s1251,n,NULL,NULL);
  return out;
}
