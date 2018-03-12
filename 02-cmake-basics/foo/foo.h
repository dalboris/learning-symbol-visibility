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
