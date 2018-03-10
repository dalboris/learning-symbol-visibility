# 02 - CMake Basics

In `01-gcc-basics`, we have seen how to control the visibility of a function
when compiling a shared library on Linux using GCC. Unfortunately, different
compilers and platforms handle this differently, therefore we need a more
robust solution. We see this solution here.

# 1. Compiling the shared library

We'll have a master `CMakeLists.txt` in this directory, and two subdirectories
`foo` and `bar` with their respective `CMakeLists.txt`. Let's start simple with
only the `foo` subdirectory and no explicit visibility:


./CMakeLists.txt:

```
cmake_minimum_required(VERSION 3.1.0)
add_subdirectory(foo)
```

./foo/CMakeLists.txt:

```
add_library(foo SHARED foo.cpp)
```

./foo/foo.h

```
#pragma once

int add(int a, int b);
int sub(int a, int b);
int mult(int a, int b);
```

./foo/foo.cpp

```
#include "foo.h"

int add(int a, int b)
{
    return a + b;
}

int sub(int a, int b)
{
    return a - b;
}

int mult(int a, int b)
{
    return a * b;
}
```

Compile in a `build` directory with:

```
mkdir build
cd build
cmake ../
make
```

You should get something like this on Linux:

```
$ cmake ../
-- The C compiler identification is GNU 5.4.0
-- The CXX compiler identification is GNU 5.4.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/boris/learning-symbol-visibility/02-cmake-basics/build

$ make
Scanning dependencies of target foo
[ 50%] Building CXX object foo/CMakeFiles/foo.dir/foo.cpp.o
[100%] Linking CXX shared library libfoo.so
[100%] Built target foo
```

Because we indicated to CMake that we wanted a shared library by using the
keyword `SHARED`, we can see that a `libfoo.so` file has been generated. This
file would be named differently on different platforms, for example, `foo.dll`
on Windows. Let's look what's inside using `nm`:

```
$ nm foo/libfoo.so
0000000000201020 B __bss_start
0000000000201020 b completed.7594
                 w __cxa_finalize
0000000000000560 t deregister_tm_clones
00000000000005f0 t __do_global_dtors_aux
0000000000200e68 t __do_global_dtors_aux_fini_array_entry
0000000000201018 d __dso_handle
0000000000200e78 d _DYNAMIC
0000000000201020 D _edata
0000000000201028 B _end
000000000000069c T _fini
0000000000000630 t frame_dummy
0000000000200e60 t __frame_dummy_init_array_entry
0000000000000778 r __FRAME_END__
0000000000201000 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
00000000000006a8 r __GNU_EH_FRAME_HDR
0000000000000518 T _init
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
0000000000200e70 d __JCR_END__
0000000000200e70 d __JCR_LIST__
                 w _Jv_RegisterClasses
00000000000005a0 t register_tm_clones
0000000000201020 d __TMC_END__
0000000000000660 T _Z3addii
0000000000000674 T _Z3subii
0000000000000686 T _Z4multii
```

In particular, we have:

```
0000000000000660 T _Z3addii
0000000000000674 T _Z3subii
0000000000000686 T _Z4multii
```

This means that all our symbols are visible (indicated by the `T`), like the
default in GCC.


# 2. Hiding all symbols by default

Let's switch to hiding all symbols by default, which is a better practice. We
have seen in `01-gcc-basics` that using GCC, this can be done by using the
`-fvisibility=hidden` option. Using CMake, the equivalent is to call
`set(CMAKE_CXX_VISIBILITY_PRESET hidden)`, or `set_target_properties(foo
PROPERTIES CXX_VISIBILITY_PRESET hidden)` if you only want to affect the `foo`
library.

./CMakeLists.txt:

```
cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
add_subdirectory(foo)
```

We can now recompile:

```
$ make
-- Configuring done
-- Generating done
-- Build files have been written to: /home/boris/learning-symbol-visibility/02-cmake-basics/build
[ 50%] Building CXX object foo/CMakeFiles/foo.dir/foo.cpp.o
[100%] Linking CXX shared library libfoo.so
[100%] Built target foo
```

And check that the symbols are now hidden:

```
$ nm foo/libfoo.so
0000000000201020 B __bss_start
0000000000201020 b completed.7594
                 w __cxa_finalize
00000000000004e0 t deregister_tm_clones
0000000000000570 t __do_global_dtors_aux
0000000000200e68 t __do_global_dtors_aux_fini_array_entry
0000000000201018 d __dso_handle
0000000000200e78 d _DYNAMIC
0000000000201020 D _edata
0000000000201028 B _end
000000000000061c T _fini
00000000000005b0 t frame_dummy
0000000000200e60 t __frame_dummy_init_array_entry
00000000000006f8 r __FRAME_END__
0000000000201000 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
0000000000000628 r __GNU_EH_FRAME_HDR
00000000000004a0 T _init
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
0000000000200e70 d __JCR_END__
0000000000200e70 d __JCR_LIST__
                 w _Jv_RegisterClasses
0000000000000520 t register_tm_clones
0000000000201020 d __TMC_END__
00000000000005e0 t _Z3addii
00000000000005f4 t _Z3subii
0000000000000606 t _Z4multii
```
