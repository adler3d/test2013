var f=(o,n)=>POST.data=POST.data.split(o).join(n);
f("};__asm{","};\n__asm{");
f("} ","};\n");
f("}; ","};\n");
f("; label_",";\nlabel_");
f(": ",":\n");
f(":_",":\n_");
f("};goto","};\ngoto");
f(";l","};\nl");
f(";_","};\n_");
f("}_","};\n_");
return POST.data;