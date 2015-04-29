/**
* Virtural CPU Designed by Braxton Nunnally
*
* Core: 0
* Model: RMD 8208
* Production Date: 3-15
* IFT: 4
* Architecture: RMDv1
*
* The RMD 8208 Microproccessor features a simple
* single core, 1 thread execution channel with a very loose
* structure with no syatax checcking, data protection,
* security, ect.
*
* This microprocessor was designed as the foundation of
* developing the Scorpion VM as a whole.
* 
*
* For full info visit "https://github.com/AndroDevcd/RPVM/blob/master/LINEUP.md"
*/

#include <stdio.h>
#include <time.h>
#include <iostream>
#include "../../var.h"
#include "core0.h"
#include "../../Ram/ram.h"
#include "gate.h"
#include <sstream>
#include "../../program.h"
#include "../../System.h"
#include "cpuf.h"
#include "runtime_exception.h"
#include "../../Log/filter.h"
#include "../x86Disassm/disassembler.h"
#include "../../Log/Log.h"
#include <string>
using namespace std;

long EAX, TMP, IP, EBX, SDX, SFC, SCX, BP, EXC, PS, LG, LSL, I1, I2, SCR, AI, IPI;

clock_t tStart;
clock_s t_clock;
bool _0Halted;
bool pass = false;
int passed_if = 0;
bool run = false;
bool scmnd = false;
bool ignore = false;
bool if_ignore = false;
bool waiting = false;

long *id;
long auto_ipi;
long IPH, IPL;
void fetch();
void execute();

void x86Shutdown();

Disassembler disasm;
Log log;

long L1_ICache_length = 1024000;// to be used else where
#define L1_Cache_Size 1024000 // 1024kb L1 Instruction Cache
string L1_ICache[ L1_Cache_Size ];

/* Instruction Set 4 */
   string i1 = "";
   string i2 = "";
   string i3 = "";
   string i4 = "";

bool reverse(bool cstate)
{
    if(!cstate)
      return true;
    else 
      return false;
}

bool C0::ACTIVE()
{
  return reverse(_0Halted);
}

void C0::Reset()
{
  log.v("System","Arm I-4 CPU core boot");
  log.i("System","Wiping  Registers..");

  EAX = 0;
  TMP = 0;
  EBX = 0;
  SDX = 0;
  SFC = 0;
  BP  = 0;
  EXC = 1;  // standard program close
  PS  = 0;
  IPI = 0;
  LG  = 1; // logging
  LSL = 2; // verbose
  I1  = 0;
  AI  = 0;
  I2  = 0;
  SCR = 0; // system call i/o response code

  t_clock.ticks = 0;
  t_clock.nanos = 0;
  t_clock.sec = 0;
  t_clock.min = 0;
  t_clock.hrs = 0;

  id =  new long[ 4 ];
  id[0] = 8008; // processor id
  id[1] = 1; // # of cores
  id[2] = 4; // IFT
  id[3] = 315; // Production date
}

void C0::Halt()
{
  _0Halted = true;
  log.v("System","Arm I-4 CPU halt");
  log.i("System","Wiping  Registers..");

  EAX = 0;
  TMP = 0;
  SDX  = 0;
  BP  = 0;
  PS  = 0;
  LG  = 0;
  IPI = 0;
  AI  = 0;
  LSL = 0;
  SCX = 0;
  SCR = 0;
}

/* Methods used to easily talk to the ram */
double C0::getr(short cell_switch, long _addr)
{
     Ram ram;
     ram.CB = 2; // E
     ram.addr((long) _addr, false);
     ram.cell(cell_switch);

     return ram.data(0.0); // get data from ram
}

void C0::setr(short cell_switch, long _addr, double data)
{

    Ram ram;
    ram.CB = 1; // S
    ram.addr((long) _addr, false);
    ram.cell(cell_switch);

    ram.data(data);   // set data to ram
}


int C0::GetVirturalAddress()
{
  return IP;
}

C0 _x86;
void C0::Interrupt(double offset)
{
    if(_0Halted){
       _0Halted = false;
       _x86.Reset();
    }
     IP = (long) offset;
       fetch();
       execute();
}

Ram _Ram;
string prog(int set_enable, long index, string data)
{

        if(index < L1_Cache_Size)
           return L1_ICache[ index ]; // super fast instruction fetching
        else {
           _Ram.CB = set_enable; // E
           _Ram.addr(index, true);
           prog_data = data;
           _Ram.cell(5);
           _Ram.data(0.0);
           return prog_data;
        }
}

int memstat;
void fetch()
{
   if(AI != 0){
      if((t_clock.ticks % AI) == 0){
         auto_ipi = IP; // store previous ip pos
         IP = IPI;    // tell cpu to scamper off and do something random(usually used in multitasking)
      }
   }

   memstat = _Ram.prog_status(IP);
   if(memstat == Ram::DONE){
          if(!run){
             printf("Time taken: %.3fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
             p_exit();
          }
          else {
             while(true)
             {
               // do not shut down
               // run forever
             }
          }
   }
   else if(memstat == Ram::RUNNING){
      i1          = prog(2, IP++, "");
      i2          = prog(2, IP++, "");
      i3          = prog(2, IP++, "");
      i4          = prog(2, IP++, "");
      execute();
      return;
   }
   else {
    RuntimeException re;
    re.introduce("ProgramStateUndetectableException","hardware faliure: cannot determine the current state of the program");
   }
}

Gate gate;
void execute() // The Decode process is inside the execute method(for performance concerns)
{
    if(_0Halted && (disasm.disassemble(i1) == 35)) {
      if(ignore)
        ignore = false;
        gate.route(disasm.disassemble(i1), disasm.disassemble(i2), disasm.disassemble(i3), disasm.disassemble(i4));
    }
    else if(_0Halted)
       return;
    t_clock.ticks++;

  if(scmnd)
   cout << "processing operands {0:" << disasm.disassemble(i1) << "} {1:" << disasm.disassemble(i2) << "} {2:" 
        << disasm.disassemble(i3) << "} {3:" << disasm.disassemble(i4) << "}" << endl;
   gate.route(disasm.disassemble(i1), disasm.disassemble(i2), disasm.disassemble(i3), disasm.disassemble(i4));
}

void C0::run0()
{
 tStart = clock();
 while(System::Running)
       fetch();
}
