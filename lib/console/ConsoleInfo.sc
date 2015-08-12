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
* Console info will hold information about the console.
*/

public class ConsoleInfo:
   public ConsoleInfo(): end
   
   public function getColorStatus():
     int requestColor = 1;
     asm(color_status, 0, "rmov sdx ConsoleInfo.requestColor\n" +			 
                          "invoke 0x84 0\n" + 
                          "r_mv scr ConsoleInfo.requestColor");
     run_asm(color_status);                     
   end
   
   @ Set current console output text color
   public function setColorStatus(int color):
     int colorShade = 1;
     asm(color_status, 0, "rmov sdx ConsoleInfo.colorShade\n" +
                          "print '<col,color>/n'");			 
     run_asm(color_status);                     
   end

   
    @ Set current console output text color and shade value
   public function setColorStatus(int shade, int color):
     asm(color_status, 0, "rmov sdx ConsoleInfo.shade\n" +
                          "print '<col,color>/n'");
     run_asm(color_status);
   end
endclass
