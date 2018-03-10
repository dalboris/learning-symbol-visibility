In this example, we learn how to make a function visible or hidden when compiling a
shared library with gcc. We will compile a library called 'foo' and use it in a
program called 'bar'.

## 1. Compiling foo

In order to compile 'foo.cpp' into a shared library, we need to compile the
source code using the -fPIC option, and link the object code using the -fPIC and
-shared options. See [this great
article](http://www.microhowto.info/howto/build_a_shared_library_using_gcc.html)
for more details.

```
g++ -c -fPIC -o foo.o foo.cpp
g++ -shared -fPIC -o libfoo.so foo.o -lc
```
