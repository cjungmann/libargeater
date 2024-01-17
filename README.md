# ARGEATER

A lame name, but I didn't want to reuse _readargs_ or adopt another
widely-used name.

While useable, this project is to be considered under development
until I've had more time to iron out the interface.  It's only
through using it that I can refine the design to make it clear to
read and use.

The project is being designed on Linux, it should work on BSD (though
not yet tested there).  I currently have no plans to make it available
for Windows.

## INSTALLATION AND USE

With the above warning in mind, feel free to download, build and
use this library.

The following commands will clone the project, then build the library
and a few test programs.

~~~sh
clone https://github.com/cjungmann/argeater.git
cd argeater
make
~~~

Install the library, header, and man page with

~~~sh
sudo make install
~~~

and uninstall with

~~~sh
sudo make uninstall
~~~

 to remove all traces.

## HELP RESOURCES

The `Makefile` includes a `help` target that enumerates the supported
targets.

There is a **man** page (argeater.3) with a synopsis of all public
functions, structs, and Macros, some functions documentation and an
example section with simple code for a working prototype.  The man
page will be universally accessible if the library is installed.  If
the library is not installed, the man page can be accessed in the
project directory by typing:

~~~sh
man -l argeater.3
~~~


## TEST PROGRAMS

The `Makefile` includes a `test` target that builds test programs.
If a C source file has a `test_` prefix, the `Makefile` will compile
the source file with the output name being the text following the
`test_` prefix.  For example, file `test_simple.c` will result in
an executable file, **simple**.  If you follow this convention while
making other self-contained source files, the `Makefile` will attempt
to compile them.

There are at least three `test_*.c` files,
- test_example.c  
  Makes `example`, which contains the code from the man page.

- test_simple.c  
  Makes `simple` with a somewhat more complete example program,
  including a custom argument interpreter that interprets a
  comma-separated set of integers.

- test_main.c  
  Makes `main`, that contains a larger assortment of AE_ITEM examples
  that attempt to document usage through examples.  Be sure to
  compile the code and look at the __-h__ or __--help__ option.

