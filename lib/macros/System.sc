import console.PrintStream;
import util.Log;

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
* System is a class that represents the Virtual machine system that runs Scorpion
* source code.
*
*/
public class System:
   public System(): end
   
   PrintStream console; @ The standard output variable

   Log log;   

   class Atrix_X1:
      public Atrix_X1(): end

      /* Internal Vm processor registers */
      int sdx, eax, ebx, sfc, scx, 
          i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, 
          i11, i12;

      function int updateRegister(string register):
        if(register.equals("sdx")):
           asm(reg_update, "rmov sdx System.Atrix_X1.sdx");
           run_asm(reg_update);
        end
        else if(register.equals("eax")):
           asm(reg_update, "rmov sdx System.Atrix_X1.eax");
           run_asm(reg_update);
        end
        else if(register.equals("ebx")):
           asm(reg_update, "rmov sdx System.Atrix_X1.ebx");
           run_asm(reg_update);
        end
        else if(register.equals("sfc")):
           asm(reg_update, "rmov sdx System.Atrix_X1.sfc");
           run_asm(reg_update);
        end
        else if(register.equals("scx")):
           asm(reg_update, "rmov sdx System.Atrix_X1.scx");
           run_asm(reg_update);
        end
        else if(register.equals("i1")):
           asm(reg_update, "rmov sdx System.Atrix_X1.i1");
           run_asm(reg_update);
        end
        else if(register.equals("i2")):
           asm(reg_update, "rmov sdx System.Atrix_X1.i2");
           run_asm(reg_update);
        end
        else if(register.equals("i3")):
           asm(reg_update, "rmov sdx System.Atrix_X1.i3");
           run_asm(reg_update);
        end
        else if(register.equals("i4")):
           asm(reg_update, "rmov sdx System.Atrix_X1.i4");
           run_asm(reg_update);
        end
        else if(register.equals("i5")):
           asm(reg_update, "rmov sdx System.Atrix_X1.i5");
           run_asm(reg_update);
        end
        else if(register.equals("i6")):
           asm(reg_update, "rmov sdx System.Atrix_X1.i6");
           run_asm(reg_update);
        end
        else if(register.equals("i7")):
           asm(reg_update, "rmov sdx System.Atrix_X1.i7");
           run_asm(reg_update);
        end
        else if(register.equals("i8")):
           asm(reg_update, "rmov sdx System.Atrix_X1.i8");
           run_asm(reg_update);
        end
        else if(register.equals("i9")):
           asm(reg_update, "rmov sdx System.Atrix_X1.i9");
           run_asm(reg_update);
        end
        else if(register.equals("i10")):
           asm(reg_update, "rmov sdx System.Atrix_X1.i10");
           run_asm(reg_update);
        end
        else if(register.equals("i11")):
           asm(reg_update, "rmov sdx System.Atrix_X1.i11");
           run_asm(reg_update);
        end
        else if(register.equals("i12")):
           asm(reg_update, "rmov sdx System.Atrix_X1.i12");
           run_asm(reg_update);
        end
        else:
          return -1;
        end
        return 0;
      end

   endclass  

endclass
