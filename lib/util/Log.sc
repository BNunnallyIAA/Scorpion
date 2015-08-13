
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
* Log is a class that will be used to log all data in an application.
*/
public class Log:
   public Log(): end

   int a(string tag, string message):
      return print(LogManager.ASSERT,tag,message);
   end

   function int e(string tag,string message):
      return print(LogManager.ERROR,tag,message);
   end

   function int w(string tag,string message):
      return print(LogManager.WARN,tag,message);
   end

   function int i(string tag,string message):
      return print(LogManager.INFO,tag,message);
   end

   function int d(string tag,string message):
      return print(LogManager.ERROR,tag,message);
   end

   function int v(string tag,string message):
      return print(LogManager.VERBOSE,tag,message);
   end

   @ Log the data
   function int println(string TAG, string message):

   end

   function int print(int priority, string tag, string message):
      if(LogManager.isLogEnabled()):
         if(LogManager.getPriority() <= priority):
            return println(tag,message);
         end
         else:
            return 0;
         end
      end
      else:
         return 0;
      end
   end

endclass
