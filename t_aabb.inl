struct t_aabb{
  vec2d min,max;
  static t_aabb make(vector<vec2d>&arr){
    t_aabb out;
    if(arr.empty())return out;
    auto&a=out.min;auto&b=out.max;a=arr[0];b=arr[0];
    for(auto&ex:arr){a.comin(ex);b.comax(ex);}
    return out;
  }
  vec2d wh()const{return max-min;}
  vec2d center()const{return 0.5*(min+max);}
};
