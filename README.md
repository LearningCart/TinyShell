# TinyShell
Tiny shell project
It creates tiny shell over serial/console.
Can be exetended as script interpreter for arduino.,
e.g.,
set pin out
set pin 10
reset pin 10

read pin 11
if pin 11 on set reset pin 10

TODO:
Port it to couple of hardware and build up real use-case.

Following things are pending:
1)  case conversion to lower
2)  command trimming etc before starting
3)	history...,
4)  Argument passing to command
5)	Optimize the mechanism to use single entry for multiple strings..,
