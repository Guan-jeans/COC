export LLVM_HOME=/home/xxx/llvm-10.0.0-built/usr/local/bin
export PATH=$LLVM_HOME:$PATH

clang -I ../common -I ../utilities -D CGRA_COMPILER -target i386-unknown-linux-gnu -c -emit-llvm -O1 -fno-tree-vectorize -fno-unroll-loops $1.c -S -o $1.ll

opt -gvn -mem2reg -memdep -memcpyopt -lcssa -loop-simplify -licm -loop-deletion -indvars -simplifycfg -mergereturn -indvars $1.ll -S -o $1_gvn.ll

para1=
para2=

if [ -n "$3" ]
then
   para1=-$3
fi

if [ -n "$4" ]
then
   para2=-$4
fi


opt -load ../../build/llvm-pass/libCDFGPass.so $para1 $para2 -fn $2 -cdfg $1_gvn.ll -S -o $1_cdfg.ll
