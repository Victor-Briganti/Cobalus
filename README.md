# COBALU COMPILER
In my journey to write a better programming language i first try to write a 
language that really in a interpreter for everything. Now in this new version of
the Cobalu(yes I remove the 'S'), this language is being executed inside a 
virtual machine.

In this new version of the language the programs are compiled in instructions
for my virtual machine and being executed in a stack environment. This made the
language way faster than his previous version(gotta go fast) that can be seen
in the draft.

## Compiling & Executing
To compile this compiler (and who compiles the compiler?) you need to have 
installed clang or g++ and make. By default the makefile is configured to 
compile using clang but if you want to use g++ just change the Makefile header 
from "CC = clang++" to "CC = g++"

Compiling:
'''
$ make
$ make debug # if you want to be able to debug the language with lldb or gdb
'''

In the archives of this compiler there is some tests files that I use to 
test the correct execution of the program, but you can write a file and 
execute it.

OBS: files do not need to be preceded by a .cpp or anything like that

Executing:
'''
$ ./cobalus <your_file>
'''

OBS: if you want to see the stack of execution of your program you can enable
it uncommenting the "#define DEBUG" line in the ./srd/Headers/global.h file.

### END
It's definetly not perfect as a language could be better but the whole point,
was to make the language faster(and It works). But I have to say that this
new version of the language has a little problem, the VM is inside a array
so it doesn't have a lot of memory to execute programs, so be careful about 
that.

But beside this I really hope you enjoy my little toy language, and I'm all
ears if you want to make the project better or has a bug just contact me. I will
be happy to help :).
