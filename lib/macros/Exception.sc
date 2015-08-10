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
* Exception is the base class for throwing exceptions.
*
*/
public class Exception:

   public function break(Class klass, string message):
     String klas = klass.tostring();
     asm(ethrow, 0):
       throw Exception.klas, Exception.message
     end
     run_asm(ethrow);
   end
   
   public Exception(string msg): break(this, msg); end
   
  
endclass
