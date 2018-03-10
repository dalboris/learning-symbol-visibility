# 01 - GCC Basics

In this example, we learn how to make a function visible or hidden when
compiling a shared library with gcc. We will compile a library called `foo` and
use it in a program called `bar`.

## 1. Compiling a shared library

Our library `foo` is made of three functions declared in `foo.h` and defined in
`foo.cpp`:

```
int add(int a, int b);
int sub(int a, int b);
int mult(int a, int b);
```

In order to compile these into a shared library, we need to compile the source
code `foo.cpp` using the -fPIC option, and link the resulting object code
`foo.o` using the -fPIC and -shared options:

```
g++ -c -fPIC -o foo.o foo.cpp
g++ -shared -fPIC -o libfoo.so foo.o -lc
```

(see for example [this
article](http://www.microhowto.info/howto/build_a_shared_library_using_gcc.html)
for more details)

Let's look at what's inside the resulting shared library `libfoo.so` by using
`nm`:

```
$ nm libfoo.so
0000000000201020 B __bss_start
0000000000201020 b completed.7594
                 w __cxa_finalize
0000000000000520 t deregister_tm_clones
00000000000005b0 t __do_global_dtors_aux
0000000000200e78 t __do_global_dtors_aux_fini_array_entry
0000000000201018 d __dso_handle
0000000000200e88 d _DYNAMIC
0000000000201020 D _edata
0000000000201028 B _end
000000000000065c T _fini
00000000000005f0 t frame_dummy
0000000000200e70 t __frame_dummy_init_array_entry
0000000000000738 r __FRAME_END__
0000000000201000 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
0000000000000668 r __GNU_EH_FRAME_HDR
00000000000004e0 T _init
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
0000000000200e80 d __JCR_END__
0000000000200e80 d __JCR_LIST__
                 w _Jv_RegisterClasses
0000000000000560 t register_tm_clones
0000000000201020 d __TMC_END__
0000000000000620 T _Z3addii
0000000000000634 T _Z3subii
0000000000000646 t _Z4multii
```

Importantly, we can see our three functions `add`, `sub`, and `mult` at the end:

```
0000000000000620 T _Z3addii
0000000000000634 T _Z3subii
0000000000000646 t _Z4multii
```

(Learn about [name mangling](https://en.wikipedia.org/wiki/Name_mangling) if you
wonder why the symbol name for "add" is "_Z3addii")

What's important for us is to notice that `add` and `sub` have the symbol type
`T`, while `mult` has the symbol type `t` (see
[here](https://www.mkssoftware.com/docs/man1/nm.1.asp) for a list of all
possible symbol types).

What are those, and why are they different? `T` means that the symbol is
exported (that is, available for clients of the shared library), while `t` means
that the symbol is hidden (that is, not available for clients of the shared
library). The reason these three functions don't have the same visibility is
that `foo.h` has actually explicitly declared these symbols with different
visibility. Here is the full content of `foo.h`:

```
#pragma once

int add(int a, int b);

__attribute__ ((visibility ("default")))
int sub(int a, int b);

__attribute__ ((visibility ("hidden")))
int mult(int a, int b);
```

The `"default"` visibility means that we want to export the symbol (yes, the
chosen nomenclature is really confusing, "default" really means "visible", or
"exported"), and the `"hidden"` attribute means that we do not want to export
the symbol.

This explains why `sub` is exported and `mult` is hidden, as we can see here:

```
0000000000000634 T _Z3subii
0000000000000646 t _Z4multii
```

Now, you can notice that we didn't indicate explicitly any visibility for `add`.
Therefore, the symbol has also been exported, which is the default in GCC:

```
0000000000000620 T _Z3addii
```

## 2. Compiling a shared library with -fvisibility=hidden

We have seen that using GCC, by default all symbols are exported unless
explicitly hidden. However, it is considered a better practice to do the
opposite: hide everything by default, unless explicitly exported.

We can do this by using the `-fvisibility=hidden` option of GCC both at the
compiling step and the linking step:

```
g++ -fvisibility=hidden -c -fPIC -o foo.o foo.cpp
g++ -fvisibility=hidden -shared -fPIC -o libfoo.so foo.o -lc
```

The output of `nm` is now the following:

```
$ nm libfoo.so
0000000000201020 B __bss_start
0000000000201020 b completed.7594
                 w __cxa_finalize
0000000000000500 t deregister_tm_clones
0000000000000590 t __do_global_dtors_aux
0000000000200e78 t __do_global_dtors_aux_fini_array_entry
0000000000201018 d __dso_handle
0000000000200e88 d _DYNAMIC
0000000000201020 D _edata
0000000000201028 B _end
000000000000063c T _fini
00000000000005d0 t frame_dummy
0000000000200e70 t __frame_dummy_init_array_entry
0000000000000718 r __FRAME_END__
0000000000201000 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
0000000000000648 r __GNU_EH_FRAME_HDR
00000000000004c0 T _init
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
0000000000200e80 d __JCR_END__
0000000000200e80 d __JCR_LIST__
                 w _Jv_RegisterClasses
0000000000000540 t register_tm_clones
0000000000201020 d __TMC_END__
0000000000000600 t _Z3addii
0000000000000614 T _Z3subii
0000000000000626 t _Z4multii
```

As we can see, besides a few address changes, the only change is that:

```
0000000000000620 T _Z3addii
```

has become:

```
0000000000000600 t _Z3addii
```

This means that `add` is now hidden, since we did not explicitly mark it as
exported. This is the behavior we use on all the other examples in this
repository.

## 3. Using the shared library in a program

Let's try to use the `foo` library in a `bar` program. We write the following
`bar.cpp` file:

```
#include <iostream>
#include "foo.h"

int main()
{
    int a = 5;
    int b = 3;

    std::cout << "a + b = " << add(a, b) << std::endl;
    std::cout << "a - b = " << sub(a, b) << std::endl;
    std::cout << "a * b = " << mult(a, b) << std::endl;

    return 0;
}
```

and compile it with:

```
g++ -o bar bar.cpp -lfoo
```

Ooops, we get the following errors!

```
/tmp/cckRuPpH.o: In function `main':
bar.cpp:(.text+0x22): undefined reference to `add(int, int)'
bar.cpp:(.text+0x59): undefined reference to `sub(int, int)'
bar.cpp:(.text+0x90): undefined reference to `mult(int, int)'
/usr/bin/ld: bar: hidden symbol `_Z4multii' isn't defined
/usr/bin/ld: final link failed: Bad value
collect2: error: ld returned 1 exit status
```

Indeed, we didn't tell GCC where the shared library was, so let's
add the current directory `$PWD` in the library path, and ask to link with
the `foo` library:


```
g++ -o bar bar.cpp -L$PWD -lfoo
```

Ooops, still not happy!

```
/tmp/ccsEWmn5.o: In function `main':
bar.cpp:(.text+0x22): undefined reference to `add(int, int)'
bar.cpp:(.text+0x90): undefined reference to `mult(int, int)'
/usr/bin/ld: bar: hidden symbol `_Z4multii' isn't defined
/usr/bin/ld: final link failed: Bad value
collect2: error: ld returned 1 exit status
```

Hum, indeed, now GCC can find `sub`, but still can't use `add` and `mult` since
they are hidden! Of course, in this simple example, we could simply add them to
the API with `__attribute__ ((visibility ("default")))`, but let's assume that
we cannot change the `foo` library and that the library author had good reasons
not to expose these functions. This means that we have no other choice than to
change our `bar` program:

```
#include <iostream>
#include "foo.h"

int main()
{
    int a = 5;
    int b = 3;

    // Can't use add(a, b) and mult(a, b): they're not part of foo's API! :(
    std::cout << "a + b = " << a + b << std::endl;
    std::cout << "a - b = " << sub(a, b) << std::endl;
    std::cout << "a * b = " << a * b << std::endl;

    return 0;
}
```

We can now successfully compile using:

```
g++ -o bar bar.cpp -L$PWD -lfoo
```

Let's run it!

```
$ ./bar
./bar: error while loading shared libraries: libfoo.so: cannot open shared
object file: No such file or directory
```

Duh, still an error!

This is because the program needs to load the `foo` shared library at runtime,
but doesn't know where the library is. In Linux, this is typically done using
the [`rpath`](https://en.wikipedia.org/wiki/Rpath). In this example, we simply
adds the current directory to the `rpath` of the `bar` program during
compilation:

```
g++ -o bar bar.cpp -L$PWD -lfoo -Wl,-rpath,$PWD
```

Few, it's finally working!

```
$ ./bar
a + b = 8
a - b = 2
a * b = 15
```
