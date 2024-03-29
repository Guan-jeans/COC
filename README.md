CO-Compiler
=======================

Frontend application compiler that generates CDFG from C code based on LLVM


## Getting Started

### Dependencies

1. LLVM-10.0.0 with Polly included

2. CMake

3. C++-17



### LLVM with Polly

1. download llvm source codes from https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-10.0.0.tar.gz
    
   extract and change directory name

```sh
    tar xvf llvmorg-10.0.0.tar.gz
    mv llvmorg-10.0.0 llvm-project-10.0.0
```

2. build llvm

```sh
    mkdir llvm-10.0.0-built
    cd llvm-project-10.0.0
    mkdir build
    cd build
    cmake -DLLVM_ENABLE_PROJECTS='polly;clang' -G "Unix Makefiles" ../llvm
    # multi-thread consumes lots of memory, e.g. -j4 : 30G+
    make -j4
    # DESTDIR set install directory
    make install DESTDIR=/xxx/llvm/llvm-10.0.0-built
```

3. set llvm env

```sh
    # set the LLVM path in the CMakeLists.txt
    set(LLVM_INCLUDE_DIRS "/xxx/llvm-10.0.0-built/usr/local/include")
    set(LLVM_LIBRARY_DIRS "/xxx/llvm-10.0.0-built/usr/local/lib")
    # set the LLVM path in the benchmark/compile.sh
    export LLVM_HOME=/xxx/llvm/llvm-10.0.0-built/usr/local/bin
    export PATH=$LLVM_HOME:$PATH
```

### Build

Using the script build.sh in this app-compiler directory

```sh
./build.sh
```

### Run

Run all benchmarks, You can forbid the dedicated extractions by parameters in parentheses

```sh
cd benchmark
./run.sh (noACC) (noPattern)
```

Or run your own single benchmark foo.c
```
functionName(){
    loop_begin();
    for(){
        ...
    }
    loop_end();
}
```

```sh
cd path_to_foo.c
compile.sh foo functionName (noACC) (noPattern)
```
