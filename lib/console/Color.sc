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
* Color is a class file that manipulates the current color of the console.
*/
public class Color:
   public Color(): end

   int loghtShade = 1;  
   int regularShade = 0;
   int normal = 38;           
   int red = 31;                
   int black = 30;
   int darkGray = 30;
   int blue = 34; 
   int green = 32; 
   int cyan = 36;
   int purple = 35;
   int brown = 33;
   int yellow = 33;
   int white = 37;
   int lightGray = 37;   

   private function int getCurrColor():
     ConsoleInfo info;
	 info = new ConsoleInfo();
	 return info.getColorStatus();			 
   end
   
   public function int getCurrentSystemColor():
     return getCurrColor();
   end   
   
   public function changeSystemColor(int color){
   
   }
   
endclass
