# 02 - CMake Basics

In `01-gcc-basics`, we have seen how to control the visibility of a function
when compiling a shared library on Linux using GCC. Unfortunately, different
compilers and platforms handle this differently, therefore we need a more
robust solution. In this repository, we make the choice of CMake and a
handful macros as a possible solution.

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

Since we indicated to CMake that we wanted a shared library (by using the
keyword `SHARED`), we can see that a `libfoo.so` file has been generated. This
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

```
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
```
or

```
set_target_properties(foo PROPERTIES CXX_VISIBILITY_PRESET hidden)
```

if you only want to affect the `foo` library. Here, let's choose to affect all
libraries, which means that we are now using the following CMakeLists at the
root of the project:

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


# 3. Exposing symbols selectively

The idea is to add the equivalent of `__attribute__ ((visibility ("default")))`
to the symbols we want to add in the API, in a cross-platform way.

First of all, instead of using directly `__attribute__ ((visibility
("default")))` and `__attribute__ ((visibility ("hidden")))`, we can hide this
implementation detail in macros, like so:

```
#pragma once

#define FOO_API __attribute__ ((visibility ("default")))
#define FOO_LOCAL __attribute__ ((visibility ("hidden")))

int add(int a, int b);
FOO_API int sub(int a, int b);
FOO_LOCAL int mult(int a, int b);
```

Recompiling and inspecting with `nm` gives me the expected:

```
0000000000000610 t _Z3addii
0000000000000624 T _Z3subii
0000000000000636 t _Z4multii
```

But this only works because I'm using GCC on Linux. Different
platforms/compilers use different conventions to specify visibility, such as
`__declspec(dllexport)` when using Visual C++ on Windows. Also, in some
platforms/compilers, you need to use a different specifier when you compile a
symbol of shared library (e.g., `__declspec(dllexport)`),  than when we use the
symbol later (e.g., `__declspec(dllimport)`)

The typical way to handle this (cf. [gcc
docs](https://gcc.gnu.org/wiki/Visibility)) is to define a few macros whose value
are different based on:

- which platform we are compiling,
- which compiler we use, and
- whether we are compiling the library or using it.

Many libraries (e.g., OpenCV, Qt, VGC) use similar macros with varying levels of
details, and here is what we will use:

./foo/dll.h

```
#pragma once

#if defined(_WIN32)
#   if defined(__GNUC__) && __GNUC__ >= 4 || defined(__clang__)
#       define DLL_EXPORT __attribute__((dllexport))
#       define DLL_IMPORT __attribute__((dllimport))
#       define DLL_HIDDEN
#   else
#       define DLL_EXPORT __declspec(dllexport)
#       define DLL_IMPORT __declspec(dllimport)
#       define DLL_HIDDEN
#   endif
#elif defined(__GNUC__) && __GNUC__ >= 4 || defined(__clang__)
#   define DLL_EXPORT __attribute__((visibility("default")))
#   define DLL_IMPORT __attribute__((visibility("default")))
#   define DLL_HIDDEN __attribute__((visibility("hidden")))
#else
#   define DLL_EXPORT
#   define DLL_IMPORT
#   define DLL_HIDDEN
#endif
```

The idea is that on Windows, you need to specify `dllexport` when compiling a
shared library, and `dllimport` when using a shared library. Symbols are always
hidden by default, so there is no syntax for hiding symbols. On Linux and MacOS,
we can use the visibility attributes that we have seen in `01-gcc-basics`, as
long as we are compiling with Clang or GCC (> 4.0).

For now, let's just care about compiling the library. For consistency with the
behaviour on Windows, we hide all symbols by default on Linux/MacOS too by using
`set(CMAKE_CXX_VISIBILITY_PRESET hidden)` in the CMakeLists.txt, and we can now
selectively make some symbols visible using `DLL_EXPORT`:

./foo/foo.h:

```
#pragma once

#include "dll.h"
#define FOO_API DLL_EXPORT
#define FOO_LOCAL DLL_HIDDEN

int add(int a, int b);
FOO_API int sub(int a, int b);
FOO_LOCAL int mult(int a, int b);
```

We can recompile and `nm`, we get the expected visibility:

```
0000000000000610 t _Z3addii
0000000000000624 T _Z3subii
0000000000000636 t _Z4multii
```


# 4. Importing the exported symbols

This is where it gets a little tricky. Remember, on Windows, we need to use
`dllimport` when using an imported symbol. This means that when using the
headers of `foo` while compiling another library or program, the header must now
be:

```
#pragma once

#include "dll.h"
#define FOO_API DLL_IMPORT
#define FOO_LOCAL DLL_HIDDEN

int add(int a, int b);
FOO_API int sub(int a, int b);
FOO_LOCAL int mult(int a, int b);
```

Note how we are using `DLL_IMPORT` instead of `DLL_EXPORT`. But for obvious
practical reasons, we do not want to use a different set of headers depending on
whether we compile the library or use it (although it is one option: vendors of
proprietary libraries may ship headers which are different that the one used to
compile the library).

Therefore, we will tell our build system to define `FOO_EXPORTS` if and only the
library is being built. This allows to define `FOO_API` as being equal to
`DLL_EXPORT` when compiling the library, but being equal to `DLL_IMPORT` when
using the library.

./foo/CMakeLists.txt:

```
add_library(foo SHARED foo.cpp)
target_compile_definitions(foo PRIVATE -DFOO_EXPORTS)
```

./foo/foo.h:
```
#pragma once

#include "dll.h"
#if defined(FOO_EXPORTS)
#    define FOO_API DLL_EXPORT
#else
#    define FOO_API DLL_EXPORT
#endif
#define FOO_LOCAL DLL_HIDDEN

int add(int a, int b);
FOO_API int sub(int a, int b);
FOO_LOCAL int mult(int a, int b);
```

On Windows, compiling with MSVC, the above header becomes:

```
int add(int a, int b);
__declspec(dllexport) int sub(int a, int b);
int mult(int a, int b);
```

when compiling the library, but becomes:

```
int add(int a, int b);
__declspec(dllimport) int sub(int a, int b);
int mult(int a, int b);
```

when using the library. On Linux, it becomes

```
int add(int a, int b);
__attribute__((visibility("default"))) int sub(int a, int b);
__attribute__((visibility("hidden"))) int mult(int a, int b);
```

when compiling or using the library.
