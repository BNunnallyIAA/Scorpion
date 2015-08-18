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
* Scorpion Virtural Machine
* Designed and developed by Braxton Nunnally
*
* current digital microprocessor speed
* 340KHz
*/
#include <string>
#include <algorithm>
#include <iostream>
#include "ctime.h"
#include "help.h"
#include "program.h"
#include "CPU/cpu.h"
#include "bv.h"
#include "System.h"
#include "Log/Log.h"
#include "Log/Logger.h"
#include "CPU/core0/runtime_exception.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>

using namespace std;

int status;
int STACK_LEVEL = 2;
#define nullptr ((void *)0)

long mem = 2000000; // std 2mb of mem
long pmem = 6000000; // std 6mb of mem
long mmax = 3000000;
long mmin;

Log x86_log;
Logger logger1;

extern double *ram; // cell 0
extern double *lram;// cell 2
extern double *xram;// cell 3
extern string *program; // cell 5

void malloc()
{
   ram = new (nothrow) double[ mem ];
   lram = new (nothrow) double[ mem ];
   xram = new (nothrow) double[ mem ];
   program = new (nothrow) string[ pmem ];
   mmin = mem;

   if(ram == nullptr || lram == nullptr || xram == nullptr || program == nullptr){
      cout << "Error: Could not create the Scorpion Virtural Machine.\nA fatal error has occured: Program will now exit." << endl;
      exit(-1039439);
   }
}

/*
* ALLOCATION_FAILED = -3;
*/
int alloc(bool free, long size)// size(in bytes)
{
  if(free){ // free mem
      if((mem - size) < mmin){
          RuntimeException re;
          re.introduce("ScorpionOutOfMemoryError","failure to free more memory than the minimum set ammount");
       }

       double *memory;
       memory = new (nothrow) double[ mem ];
       if(memory == nullptr)
          return -3;

       for(int i = 0; i < mem; i++)
          memory[i] = ram[i];
       delete[] ram;

       ram = new (nothrow) double[ mem - size ];
       if(ram == nullptr)
          return -3;
       for(int i = 0; i < (mem - size); i++)
          ram[i] = memory[i];

       // ---------------------------------------
       for(int i = 0; i < mem; i++)
          memory[i] = lram[i];
       delete[] lram;

       lram = new (nothrow) double[ mem - size ];
       if(lram == nullptr)
          return -3;
       for(int i = 0; i < (mem - size); i++)
          lram[i] = memory[i];

       // ---------------------------------------
       for(int i = 0; i < mem; i++)
          memory[i] = xram[i];
       delete[] xram;

       xram = new (nothrow) double[ mem - size ];
       if(xram == nullptr)
          return -3;
       for(int i = 0; i < (mem - size); i++)
          xram[i] = memory[i];
       mem -= size;
  }
  else {
       if((size + mem) > mmax){
          RuntimeException re;
          re.introduce("ScorpionOutOfMemoryError","failure to allocate more memory than the max set ammount");
       }

       double *memory;
       memory = new (nothrow) double[ mem ];
       if(memory == nullptr)
          return -3;

       for(int i = 0; i < mem; i++)
          memory[i] = ram[i];
       delete[] ram;

       ram = new (nothrow) double[ mem + size ];
       if(ram == nullptr)
          return -3;
       for(int i = 0; i < mem; i++)
          ram[i] = memory[i];

// ---------------------------------------
       for(int i = 0; i < mem; i++)
          memory[i] = lram[i];
       delete[] lram;

       lram = new (nothrow) double[ mem + size ];
       if(lram == nullptr)
          return -3;
       for(int i = 0; i < mem; i++)
          lram[i] = memory[i];

// ----------------------------------------
       for(int i = 0; i < mem; i++)
          memory[i] = xram[i];
       delete[] xram;

       xram = new (nothrow) double[ mem + size ];
       if(xram == nullptr)
          return -3;
       for(int i = 0; i < mem; i++)
          xram[i] = memory[i];
       mem += size;
  }
}

void mualloc()
{
  delete[] ram;
  delete[] lram;
  delete[] xram;
  delete[] program;
}

void Restart()
{
   System::Running = false;
   CPU mprocessor;
   mprocessor.Reset();
   System::Running = true;
   System::SetupSystem();
}

void x24Shutdown()
{
   mualloc();
   System::Running = false;
   CPU mprocessor;
   mprocessor.Halt();
   Log lg;
   lg.Shutdown();
}

void Run()
{

   Alloc::scorpion_Init(stacksize, memset, "Main",
                         0, Xmax, Xmin, getimage());
   CPU mprocessor;
   System::Running = true;
   mprocessor.Reset();
   System::SetupSystem();
}

extern int arg_start;
string prog_args = "";
int arg_c = 0;
int main( int argc, const char **file )
{
  prepargs();
  if(argc >= 2)
    handleargs(argc, file);
  else if (argc == 1)
    help();

  if(argc > 2){
     stringstream ss;
     for(int i = arg_start + 2; i < argc; i++){
        ss << file[i] << " ";
     }
     prog_args = ss.str();
  }
  arg_c = argc - (arg_start + 2);

  if(arg_c == -1){ // prog was not passed
     cout << "Error: Could not create the Scorpion Virtural Machine.\nA fatal error has occured: Program will now exit." << endl;
     exit(-1039439);
  }

  status = mkdir("/usr/share/scorpion/lib", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  x86_log.prepare(VERBOSE,true);

  if(run(file) == 0)
      Run();
  else
     cout << "scorpion: fatal err occured!...check '/usr/share/scorpion/log.txt' for error and system information\nuse -help for a list of options." << endl;

    return 0;
}

