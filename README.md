Lua 5.2 Parser and Pretty Printer
=================================

This repository contains a reentrant [lua](https://www.lua.org) 5.2 parser and
pretty printer implemented in C.  It has been reasonably well tested and any
code parsed and pretty printed should execute correctly.  If not please submit
an issue.

Requirements
------------

The code requires a C compiler, make, flex and bison.  These should be readily
available on any POSIX system.  The bison based parser requires the push parser
functionality which I believe means version 3.5 or greater.

Building
--------

Build by changing into the repo directory and running make:

```bash
$ cd lua_parser
$ make
```
and to test:

```bash
$ make test
```
In order for the tests to run you will need to install
[luajit](https://luajit.org/).  It will also run fine using lua 5.2 but you
would need to make minor modifications to the test script to point to the correct
binary.  Later versions of lua should also work but I haven't tested them.

License
-------

[MIT](https://opensource.org/licenses/MIT)
