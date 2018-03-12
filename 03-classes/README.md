# 03 - Classes

C++ classes and structs can be added to the API in a similar way.

foo.h:

```
class FOO_API Foo
{
public:
    Foo(int a, int b);
    int add() const;
    int sub() const;

private:
    int a_;
    int b_;
};
```

If you compile and `nm `, you get the following:

```
[...]
0000000000000690 T _ZN3FooC1Eii
0000000000000690 T _ZN3FooC2Eii
00000000000006b4 T _ZNK3Foo3addEv
00000000000006ce T _ZNK3Foo3subEv
```

(Note: if you wonder why there are two symbols for Foo::Foo(int, int), read
[this](https://stackoverflow.com/questions/6921295/dual-emission-of-constructor-symbols))

So we can see that all methods of the class were exported, and we can use
them in `bar`:

bar.cpp:

```
int main()
{
    int a = 5;
    int b = 3;
    Foo f(a, b);

    std::cout << "a + b = " << f.add() << std::endl;
    std::cout << "a - b = " << f.sub() << std::endl;

    return 0;
}
```

```
$ make
Scanning dependencies of target foo
[ 25%] Building CXX object foo/CMakeFiles/foo.dir/foo.cpp.o
[ 50%] Linking CXX shared library libfoo.so
[ 50%] Built target foo
Scanning dependencies of target bar
[ 75%] Building CXX object bar/CMakeFiles/bar.dir/bar.cpp.o
[100%] Linking CXX executable bar
[100%] Built target bar

$ bar/bar
a + b = 8
a - b = 2
```

Note that if we remove `FOO_API` in `class FOO_API Foo`, we get the
following linking errors:

```
CMakeFiles/bar.dir/bar.cpp.o: In function `main':
bar.cpp:(.text+0x36): undefined reference to `Foo::Foo(int, int)'
bar.cpp:(.text+0x42): undefined reference to `Foo::add() const'
bar.cpp:(.text+0x76): undefined reference to `Foo::sub() const'
collect2: error: ld returned 1 exit status
bar/CMakeFiles/bar.dir/build.make:95: recipe for target 'bar/bar' failed
make[2]: *** [bar/bar] Error 1
CMakeFiles/Makefile2:140: recipe for target 'bar/CMakeFiles/bar.dir/all' failed
make[1]: *** [bar/CMakeFiles/bar.dir/all] Error 2
Makefile:83: recipe for target 'all' failed
make: *** [all] Error 2
```

Indeed, the symbols would not be exported:

```
00000000000005e0 t _ZN3FooC1Eii
00000000000005e0 t _ZN3FooC2Eii
0000000000000604 t _ZNK3Foo3addEv
000000000000061e t _ZNK3Foo3subEv
```
