struct t_${
  template<class TYPE>static void set(TYPE&,const t_$&){}
  template<class TYPE>static void set(TYPE&a,TYPE&b){a=b;}
  template<class TYPE>static void set(TYPE&a,const TYPE&b){a=b;}
  static void set(int&a,int b){a=b;}
  template<int n>
  static void set(string&a,const char(&b)[n]){a=b;}
};