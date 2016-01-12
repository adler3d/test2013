var tfs="t_fallback scope(dev,__FUNCTION__);";var std="#include \"stdafx.h\"\n";
var w="auto&O=scope.optional;";var tfswdmo="T_FALLBACK_SCOPE_WITH_DMO()";var tfsraw="T_FALLBACK_SCOPE()";
var def="#define T_FALLBACK_SCOPE_WITH_DMO()T_FALLBACK_SCOPE()auto&ok=scope.ok;auto&D=scope.mandatory;auto&M=scope.mandatory;auto&O=scope.optional;\n";
def+="#define T_FALLBACK_SCOPE()t_fallback scope(dev,__FUNCTION__);"; var id=-1;
if(POST['data'].split(std).length>2)return "fail with stdafx.h";
if(POST['data'].split(std).length==1)std="";
if(POST['data'].split(std).length==2)POST['data']=POST['data'].split(std).slice(1).join("");
return std+def+"\n"+POST['data'].split(tfs).map(ex=>{id++;if(!id)return ex;return ex.search(w)<0?tfsraw+ex:tfswdmo+ex.split(w).slice(1).join(w);}).join("");
