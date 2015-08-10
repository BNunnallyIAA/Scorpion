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
* PrintStream is a class for printing data to the console.
*/

public class PrintStream:
    public PrintStream(); 
	
	public function flush():
	   asm(flush, 0, "print '/&'"); 
	   run_asm(flush);
	end
	
	private function nwline():
	  asm(nl, 0, "print '/n'"); 
	  run_asm(nl);
	end 
	
	public function println():
	   nwline();
	end
	
	public function println(boolean x):
	   asm(print, 0, "print '<bl,PrintStream.x>'");
	   run_asm(print);
	   nwline();
	end
	
	public function println(string stream):
	   asm(print, 0, "print '<str,PrintStream.stream>'"); 
	   run_asm(print);
	   nwline();
	end
	
	public function println(Object ostream):
	   asm(print, 0, "print '<v,PrintStream.ostream>'"); 
	   run_asm(print);
	   nwline();
	end
	
	public function print();
	
	public function print(string stream):
	   asm(print, 0, "print '<str,PrintStream.stream>'"); 
	   run_asm(print);
	end
	
	public function print(Object ostream):
	   asm(print, 0, "print '<v,PrintStream.stream>'"); 
	   run_asm(print);
	end
	
	public function print(boolean stream):
	   asm(print, 0, "print '<bl,PrintStream.stream>'"); 
	   run_asm(print);
	end
endclass
