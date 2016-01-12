var tfs="t_fallback scope(dev,__FUNCTION__);"
var w="auto&O=scope.optional;";var tfswdmo="T_FALLBACK_SCOPE_WITH_DMO()";var tfsraw="T_FALLBACK_SCOPE()";
var def="#define T_FALLBACK_SCOPE_WITH_DMO()T_FALLBACK_SCOPE()auto&ok=scope.ok;auto&D=scope.mandatory;auto&M=scope.mandatory;\n";
def+="#define T_FALLBACK_SCOPE()t_fallback scope(dev,__FUNCTION__);"; var id=-1;
return def+"\n"+POST['data'].split(tfs).map(ex=>{id++;if(!id)return ex;return ex.search(w)<0?tfsraw+ex:tfswdmo+ex.split(w).slice(1).join(w);}).join();
