# PL-o-Compiler

Author: Jackie Lin

##Description

* A compiler for a tiny programming language(PL/O). In which the compiler will generate code for a virutal machine. 

* The components of these compilers consists of our Lexical Analyzer/Scanner, Parser/Code Generator, and Virtual Machine

### Executing program

* How to run the program

TO COMPILE
```
make
```
TO EXECUTE: given file(e.g. input.txt) 
```
./a.out input.txt
```

Additional supportive compiler directives:
```
-l : print the list and table of lexemes/tokens (HW2 output) to the screen
-s : print the symbol table
-a : print the generated assembly code (parser/codegen output) to the screen
-v : print virtual machine execution trace (HW1 output) to the screen
<filename>.txt : input file name, for e.g. input.txt
```
  
EX commands:
```
./a.out input.txt –l –a –v Print all types of output to the console
./a.out input.txt –v Print only the VM execution trace to the console
./a.out input.txt Print  nothing  to  the  console  except  for  program  read 
and write instructions.
```
