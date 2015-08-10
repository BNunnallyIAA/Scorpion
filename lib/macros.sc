import macros.InstructionSet;

/*
* Copyright 2015 Braxton Nunnally
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
* Scorpion Open Source Project
* This file contains classes and methods that will be used as
* library resources for the Scorpion programming language.
*
* Macros is a library thats acts as the base class for 
* all low level io. This class will have the base methods 
* for performimg simple and complex operations. All library
* classes will stem from this class.
*
*/
public class Macros:
    public Macros(): end
    
    @ Native VM addressing for class
    private int serialUID = 26437644;
    
    @ Mapp all saved VM Instructions
    private class InstructionMap: 
        int instr;
        double[] args;
        
    endclass
    
    private InstructionMap[10000] imap;
    private int index;
    
    private class InvalidInstructionException inherits Execption:
         public InvalidInstructionException(string message):
            break(this, message);
         end
    endclass
    
    private class InstructionArgumentOutOfBoundsException inherits Execption:
         public InstructionArgumentOutOfBoundsException(string message):
            break(this, message);
         end
    endclass
    
    public function add(short instr, double[] args):
      imap[index++].instr = instr;
      imap[index - 1].args = new double[args.size()];
      
      int i;
      for(i : args.size()):
           imap[index - 1].args[i] = args;
      end
    end
    
    public function run(int index):
      if(index < 0):
        @ run all instructions
        int i;
        for( i : (index + 1)):
          int instr = imap[i].instr;
          double[] arg_arry = imap[i].args;
          asm(vm_run, 0):
             adr args_adr Macros.arg_arry
             execute instr, args_adr
          end
          run_asm(vm_run);
        end
        return;
      end
      
      int instr = imap[index].instr;
      double[] arg_arry = imap[index].args;
      asm(vm_run, 0):
         adr args_adr Macros.arg_arry
         execute instr, args_adr
      end
      run_asm(vm_run);
    end 
    
    public function newInstruction(short instr, double[] args):
       InstructionSet iset = new InstructionSet();
       
       if(iset.isInstruction(instr)):
           if(iset.argOk(args.size()):
              @ map the instruction
              add(instr, args);
           end
           else:
              throw InstructionArgumentOutOfBoundsException("The instruction " + iset.name + " does not have the correct ammount of instructions");
           end
       end
       else:
          throw InvalidInstructionException("The instruction: " + instr + " does not exist!");
       end
    end
endclass
