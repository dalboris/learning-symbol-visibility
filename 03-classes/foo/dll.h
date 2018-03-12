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
