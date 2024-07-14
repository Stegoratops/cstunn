# CSTUNN

**CSTUNN** (**C** **ST**ruct and **UN**ion **N**otation) is a markup-language in the style of the struct/union of the C-language initilizer. Its primary purpose is the injection of data into already existing structs during runtime, specified in a humanreadable format. Another goal is, that a stock C-Syntax-checker shall be largely able to check the files by adding some stuff, that shall be ignored by the parser.

This library parses inputed strings and writes the parsed input to a user-specified buffer. Serialization is (currently) outside the scope of this project.

## Format

The Format begins with an opening bracket (`{`) and ends with the matching closing bracket (`}`). Any character before and after gets ignored. Inside the brackets the format matches that of the initilizer of structs and unions of the C language ([See Here](https://en.cppreference.com/w/c/language/struct_initialization) for details). With the exception that structure members **may not** be unnamed.

Some simple Example:
```c
#include "examples/04_complex_struct/complex_struct.inc"
__ {
    .an_i = 1,
    .an_f = 2,
    .i = 3,
    .f = 4,
    .named = { .i = 5, .f = 6, },
    .arr = {
        { .i = 7, .f = 8  },
        { .i = 9, .f = 10 },
        { .i = 11, .f = 12 },
    }
};
```
For more ellaborate examples, and the underlying definitions necessary see `examples/`.

## Building

Simply compile `cstunn_build.c` as a library. Make sure to add the base directory of the repo to the include-search-paths of your preprocessor (`-I.` in gcc when current directory is at base of repo). It is somewhat compatible with C89, though may require some fixes from compiler to compiler. From C99 onwards, its fully supported.

The examples can be run by compiling the `main.c` in any given folder and than executing them.
