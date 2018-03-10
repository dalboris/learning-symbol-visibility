# 01 - GCC Basics

In this example, we learn how to make a function visible or hidden when compiling a
shared library with gcc. We will compile a library called 'foo' and use it in a
program called 'bar'.

## 1. Compiling foo without -fvisibility=hidden

Our library 'foo' is made of three functions declared in foo.h and defined in
foo.cpp:

```
int add(int a, int b);
int sub(int a, int b);
int mult(int a, int b);

```

In order to compile this into a shared library, we need to compile the source
code foo.cpp using the -fPIC option, and link the resulting object code foo.o
using the -fPIC and -shared options:

```
g++ -c -fPIC -o foo.o foo.cpp
g++ -shared -fPIC -o libfoo.so foo.o -lc
```

(see [this great
article](http://www.microhowto.info/howto/build_a_shared_library_using_gcc.html)
for more details)
