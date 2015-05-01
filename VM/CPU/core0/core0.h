#ifndef VCPU 
#define VCPU 
#include <string>
#include "../../Log/Log.h"
#include "../x86Disassm/disassembler.h"
using namespace std;
extern bool _0Halted, scmnd, pass,
            ignore, if_ignore;
extern long EAX,TMP,IP, EBX, SDX, SFC, SCX, BP, EXC, PS, LG, LSL, I1, I2, SCR, AI, IPI;
extern long *id, auto_ipi, IPH, IPL,
              L1_ICache_length;
extern int passed_if;
extern string prog_data;
extern Log lg;
extern Disassembler disasm;
extern string L1_ICache[ 1024000 ];

struct clock_s { // cpu status info
   long ticks;
} ;
extern clock_s t_clock;
extern string prog(int set_enable, long index, string data);
extern bool waiting;
   class C0 {
      public:
        void Reset();
        bool ACTIVE();
        void Halt();
        void setr(short cell, long _addr, double data);
        double getr(short cell, long _addr);
        int GetVirturalAddress();
        void Interrupt(double);
        void run0();
   };

extern C0 core0;
#endif
