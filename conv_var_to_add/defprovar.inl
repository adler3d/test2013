#define ADD(TYPE,NAME,VALUE)TYPE NAME;
DEF_PRO_VARIABLE(ADD)
#undef ADD
//===
void DoReset()
{
  t_$ $;
  #define ADD(TYPE,NAME,VALUE)t_$::set(this->NAME,VALUE);
  DEF_PRO_VARIABLE(ADD)
  #undef ADD
}
#undef DEF_PRO_VARIABLE