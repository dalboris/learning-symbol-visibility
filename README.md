# Learning Symbol Visibility

When creating a shared library in C++, you have the option to "export" or "hide"
some or all your "symbols". By symbol, we mean a variable, a function, a class
name, etc. The symbols that are "exported" can be used by clients of the shared
library, while symbols that are "hidden" cannot be used. This is called "symbol
visibility". The easiest thing to do is simply to to export all your symbols,
which is the default on most compilers. However, this may potentially lead to
name conflicts and slow load time of the shared library. This is why it is
generally considered a better practice to hide all your symbols by default, and
selectively export only the symbols which you desire your clients to use. These
exported symbols define the "API" of your library.

This project contains various minimal examples showing how to control symbol
visibility. I wrote these examples while learning symbol visibility myself, 
and decided to upload them to github in the hope that it is useful to others.

All code and documentation is dedicated to the public domain.
