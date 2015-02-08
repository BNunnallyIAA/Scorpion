# RPVM
Raspberry Pi VM implementation in C++ (can run in any computer that can run c files)

# Info
This is a vm that i will be developing for quite a while that is made using the Raspberry Pi mofel B+.  The aim of this program it to make it easy to develope programs in assembly language  that solve problems alike a traditional vm would. NOTE : this vm is not is anyway like any specific computer architecture. NOTE: All commits pushed to this repository will be 90% or beter performance wise. i.e. I will not commit code that does not work properly. All code will be put through a series of trials and tests before committing.
 
I will be developing a new language called **Scorpion. The high level part i will develope last because that is the hardest portion in this vm. Wtriting programs in the vm will operate at a low level allowing the simulation of programming a computer. 

# Specifications
The vm will have an organized process for executing code. Below is a simple representation of the process of writing programs in Scorpion.

              [source] ========> High level source code 'helloworld.sc'
                 ||
              [source] =========> Low level assembly source code 'helloworld.sasm'
                 ||
           [machine code] =======> Direct 'machine' code for the vm to execute 'hello.bo'
                 ||
                [VM] =============> The virtural machiene can now run the object code
                 ||
              (output)
           
Th first part in the process is high level source code written in Scorpion. The file extention must end with .sc in order for the assembler to build a 'sasm' file. The source code in scorpion is alot alike c and java combined with some additional advantages. A 'sasm' file(scorpion assmebly) file is then generated by the language compiler. The sasm file is one step closer to machiene instructions that the vm can directly execute. The next stage is the generation of the machiene code instructions for the vm. The generated file will be a '.bo'(binary object) file that holds only 1's, 0's, and .'s. Once this file is generated you can then run the vm with the .bo file. All programs will be written and build in c++.

# Programs
Based on the specifications defined, for everthing to work there are a total of 3 programs needed to run the vm.

Name | Description
---- | -----------
scpnc | (scorpion compiler) is used to compile the high level source code down to assembly language.
sasm | (scorpion assembler) translate the assembly code to binary object format.
svm | (scorpion vm) the vm itself runs the generated .bo file(can only run one single file as the entire program).

All programs reside in the programs directory.

# How to use
To use these programs you will have to folow the main() methid restriction I have provoded. So far I only have the specifications for the vm(currently i only write programs in direct 'machine code').
In the console type:

      $  ./svm file.bo
 
The vm can only take a single object file that has all the instructions needed to run your program. In the future I plan to build a linker that will link multiple files together into a single file.  But for now you will only be able to have a single file as the full program to be run. i.e. when using the compiler you will not be able to reference other files. 

## Instruction set
Syntax for sasm is explained in SYNTAX.md
All registers are explained below
Special commands are marked with a *

Opcode | Arguments | Description
------ | --------- | -----------
halt   | bool      | Halts the vm. 'bool' wether or not to reboot the vm and empty all data in registers and reloads the program back onto the cpu.
loadi  | r# value  | Load an integer to the specified register
add    | eax[], r# r#:r# | Add the specified data sets and store it in a register
sub    | eax[], r# r#:r# | Subtract the specified data sets and store it in a register
dout   | r#        | Output the data in the register with decomal format
scmnd  |           | Tells the cpu to show the next operation
sregs  |           | Shows the special registers and the values that reside in them
loadbl | r# bool   | Loads a boolean to the specified register
same   | r# r#:r#  | Test if the data in both registers are the same and store the result in a register
size   |           | Get the size if the current running program in bytes
mv     | r#<-r#    | Move the value of one register in the specified register
cp     | r#<-t#    | Copy the contents of a register into the specified register
rm     | r#        | Wipe the data inside of a register
mult   | eax[], r# r#:r# | Multiply the specified data sets and store it in a register
div    | eax[], r# r#:r# | Divide the specified data sets and store it in a register
*func  | r#        | specify a register as a function
push   | r#        | Initalize a function and fill the funtion with operations
*return |           | Tell the cpu to jump ahead to the specified instruction
call   | r#        | Call a function 
swp    | r#:r#     | Swap the contents that are inside the registers
ssregs |           | Show the special registers and their contents
sfregs |           | Show the flags stored for each register
eof    | r#        | Dump the flags for the specified register
*loop  | r# r# (i) | Loop through a command for the specified ammount of iterations and store it in the specified register
bout   | r#        | Output the data inside the specified register in boolean format
rem    | eax[], r#  r#:r# | Get the remainder of the two operands and store them inside the specified register

# Registers
More registers will be added, the present registers are not final 

Name   | Parameters | Description
------ | ---------- | -----------
r1-r29 |  any       | The basic registers from 0 to 29 that can hold any type of data
eax    | [int,int]  | This is a special register used only for quick math arithmetic. The two values passed to this register will be evaluated directly and stored inside the register.
tmp    |  any       | This is a register that is used to temporarily hold irrelivant data
ip     |   [int]    | (instruction pointer)Tells the cpu where in the program to execute the next instruction
r0     |            | Represents main function program starts at r0

# Notes
I will program all these programs from the bottom up. i.e. first the vm, next the assembler, and finally the high level language compiler.
