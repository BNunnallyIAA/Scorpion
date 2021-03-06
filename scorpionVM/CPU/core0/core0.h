#ifndef VCPU 
#define VCPU 
#include <string>
#include <sstream>
#include "../../Log/Log.h"
#include "../x24Disassm/disassembler.h"
using namespace std;
extern bool _0Halted, scmnd, pass,
            ignore, if_ignore, in_func, in_loop,
            debugging, clear_ignore;
extern long EAX,TMP,IP, EBX, SDX, SFC, SCX, BP, EXC, PS, LG, LSL, I1, I2, SCR, AI, IPI, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12;
extern long *id, auto_ipi, IPH, IPL,
              L1_ICache_length;
extern long long cycles;
extern int passed_if;
extern string prog_data;
extern Log lg;
extern Disassembler disasm;
extern string L1_ICache[ 1024000 ];
extern double reg_check_ret(long addr);
extern void reg_check_set(long addr, double data);

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
        double GetTime();
        unsigned long long Get_UTime();
        void resetTime();
        void reset_UTime();
        stringstream buf;
   };

extern C0 core0;
#endif
