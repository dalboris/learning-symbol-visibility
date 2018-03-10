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
