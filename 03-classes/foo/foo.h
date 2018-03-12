#pragma once

#include "dll.h"
#if defined(FOO_EXPORTS)
#    define FOO_API DLL_EXPORT
#else
#    define FOO_API DLL_EXPORT
#endif
#define FOO_LOCAL DLL_HIDDEN

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
