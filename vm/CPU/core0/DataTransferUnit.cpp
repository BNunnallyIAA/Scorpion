#include "core0.h"
#include <stdlib.h>
#include "runtime_exception.h"
#include "../../System.h"
#include "../../Bus/bus.h"
#include "../../program.h"
#include "../../var.h"
#include "io.h"
#include "../../x86Disasm/disassembler.h"
#include "../../sram.h"
#include "../../Log/filter.h"
#include "../../Ports/ports.h"
#include "cpuf.h"
#include "../../Bus/bus.h"
#include "../../Log/Log.h"
#include "../../Ram/ram.h"
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

C0 C;
Log d_log;
void loadi(double *pkg)
{
    //checkreg(pkg[0]);
    //checktype(flag[ pkg[0] ],pkg[1]);
      if( C.getr(1, pkg[0]) == OI){
          C.setr(1, pkg[0], INT);
          C.setr(0, pkg[0], pkg[1]);
      }
      else if( C.getr(1, pkg[0]) == INT )
          C.setr(0, pkg[0], pkg[1]);
      else
        cout << "invalid type assignment type int cannot be assigned to type " << C.getr(1, pkg[0]) << endl;
       // log invalid type assignment
}

void rflush()
{
  Ram ram;
  for(int cell = 0; cell < ram.info(2); cell++){
      stringstream ss;
      ss << cell;
      d_log.i("System","Flushing cell [" + ss.str() + "]");
      for(int i = 0; i < ram.info(0); i++)
          C.setr(cell, i, 0);
  }
}

void c_print(double _char)
{
  if(_char == 256)
    cout << "\n";
  else if(_char == 257)
    cout << "\t";
  else if(_char == 258)
    cout << "";
  else if((_char >= 0) && (_char <= 255)) {
    char c = _char;
    cout << c;
  }
  else{
   cout << "system warning: value " << _char << " is not a char" << endl;
   EBX = 2; 
  }
}

void _print(double *pkg)
{ 
 if(pkg[0] <= 0){}
 else if(pkg[0] == 1){
   c_print(pkg[1]);
 }
 else if(pkg[0] == 2){
   c_print(pkg[1]);
   c_print(pkg[2]);
 }
 else if(pkg[0] > 2) {
    IP--;
    IP--;
    Disassembler d;
    for(int i = 0; i < pkg[0]; i++){
       string str = prog(2, IP++, ""); // get char
       long _str = d.disassemble(str); // dissasemble char
       c_print(_str); // print char
   }
 }
 else{
  cout << "CPU print_length_logic err something went wrong while determing the length of the string to print" << endl;
  EBX = 1;
  p_exit();
 }
}
bool reg = false;
void c_printf(double _char)
{
  if(_char == 256 && !ignore)
    cout << "\n";
  else if(_char == 257  && !ignore)
    cout << "\t";
  else if(_char == 258  && !ignore)
    cout << "";
  else if(_char == 0){ // reg r#
    reg = true;
    Disassembler d;
    string str = prog(2, IP++, ""); // get char
    long _str = d.disassemble(str); // dissasemble char
    //cout << "pinting from reg " << _str  << " -1: " << (_str - 1) << " +1: " << (_str + 1)<< endl;
    if(!ignore)
      cout << C.getr(0, _str);
  }
  else if((_char >= 5) && (_char <= 255)  && (!ignore)) {
    char c = _char;
    cout << c;
  }
  else {
   if(!ignore){
      cout << "system warning: value " << _char << " is not a char" << endl;
      EBX = 2;
   }
  }
}

void _printf(double *pkg)
{
 if(pkg[0] <= 0){}
 else if(pkg[0] >= 1) {
    IP--;
    IP--;
    Disassembler d;
    for(int i = 0; i < pkg[0]; i++){
       string str = prog(2, IP++, ""); // get char
       long _str = d.disassemble(str); // dissasemble char
       c_printf(_str); // print char
   }
   if((pkg[0] == 1) && !(reg))
    IP++;
   reg = false;
 }
 else{
  cout << "CPU print_length_logic err something went wrong while determing the length of the string to print" << endl;
  EBX = 1;
  p_exit();
 }
}

void loadr(double *pkg)
{
       if(pkg[1] == 21)
          C.setr(0, pkg[0], EAX);
       else if(pkg[1] == 22)
          C.setr(0, pkg[0], IP);
      else if(pkg[0] == 22)
          IP = pkg[1];
       else
          C.setr(0, pkg[0], C.getr(0, pkg[1]));
}

int ibool(long);
void loadbl(double *pkg)
{
      //checkreg(pkg[0]);
    //checktype(flag[ pkg[0] ],pkg[1]);
       if( C.getr(1, pkg[0]) == OI){
          C.setr(1, pkg[0], BOOL);
          C.setr(0, pkg[0], ibool( (long) pkg[1] ));
      }
      else if( C.getr(1, pkg[0]) == BOOL )
          C.setr(0, pkg[0], ibool( (long) pkg[1]));
      else
        cout << "invalid type assignment type bool cannot be assigned to type " << endl;
       // log invalid type assignment
}

long _char(long _ch)
{
  if(_ch >= 5 && _ch <= 258)
     return _ch;
  else 
     return 5;
}

void loadc(double *pkg)
{
     if( C.getr(1, pkg[0]) == OI){
          C.setr(1, pkg[0], CHAR);
          C.setr(0, pkg[0],(long)  _char((long) pkg[1]));
      }
      else if( C.getr(1, pkg[0]) == CHAR)
          C.setr(0, pkg[0],(long) _char((long) pkg[1]));
      else
        cout << "invalid type assignment type bool cannot be assigned to type " << endl;
       // log invalid type assignment
}

void dload(double *pkg)
{
  if( C.getr(1, pkg[0]) == OI){
          C.setr(1, pkg[0], DOUBLE);
          stringstream ss;
          ss << pkg[1] << "." << pkg[2];
          string dec = ss.str();
          C.setr(0, pkg[0], atof(dec.c_str()));
      }
      else if( C.getr(1, pkg[0]) == DOUBLE ){
          stringstream ss;
          ss << pkg[1] << "." << pkg[2];
          string dec = ss.str();
          C.setr(0, pkg[0], atof(dec.c_str()));
      }
      else
        cout << "invalid type assignment type double cannot be assigned to type " << endl;
       // log invalid type assignment
}

void mv(double *pkg)
{
       if(pkg[1] == 21){
          C.setr(0, pkg[0], EAX);
          EAX = null;
       }
       else{
          C.setr(0, pkg[0], C.getr(0, pkg[1]));
          C.setr(0, pkg[1], null);
          C.setr(1, pkg[0], C.getr(1, pkg[1]));
          C.setr(1, pkg[1], OI);
       }
}

void mov(double *pkg)
{
      switch( (long) pkg[0] )
      {
          case 0:
	    EBX = pkg[1];
           break;
  	   case 1:
	    SDX = pkg[1];
           break;
	   case 2:
	    BP = pkg[1];
           break;
 	   case 3:
	    EXC = pkg[1];
           break;
	   case 4:
	    PS = pstatus;
           break;
	   case 5:
	    LG = pkg[1];
           break;
  	   case 6:
	    LSL = pkg[1];
           break;
           case 7:
	    SFC = pkg[1];
           break;
           case 8:
            SCX = pkg[1];
           break;
           case 9:
            I1 = pkg[1];
           break;
           case 10:
            I2 = pkg[1];
           break;
           case 11:
            TMP = pkg[1];
           break;
      }
}

void r_mv(double *pkg)
{
      switch( (long) pkg[0] )
      {
          case 0:
            if(C.getr(1, pkg[1]) == INT)
                 C.setr(0, pkg[1], EBX);
            else
              cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
           break;
           case 1:
            if(C.getr(1, pkg[1]) == INT)
                 C.setr(0, pkg[1], SDX);
             else
              cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
           break;
           case 2:
             if(C.getr(1, pkg[1]) == INT)
                 C.setr(0, pkg[1], BP);
             else
              cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
           break;
           case 3:
             if(C.getr(1, pkg[1]) == INT)
                C.setr(0, pkg[1], EXC);
             else
              cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
           break;
           case 4:
            if(C.getr(1, pkg[1]) == INT)
                C.setr(0, pkg[1], PS);
            else
              cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
           break;
           case 5:
            if(C.getr(1, pkg[1]) == INT)
                C.setr(0, pkg[1], LG);
            else
              cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
           break;
           case 6:
            if(C.getr(1, pkg[1]) == INT)
                C.setr(0, pkg[1], LSL);
            else
              cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
           break;
           case 7:
            if(C.getr(1, pkg[1]) == INT)
                C.setr(0, pkg[1], SFC);
            else
              cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
           break;
           case 8:
            if(C.getr(1, pkg[1]) == INT)
                C.setr(0, pkg[1], SCX);
            else
              cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
           break;
           case 9:
            if(C.getr(1, pkg[1]) == INT)
                C.setr(0, pkg[1], I1);
            else
              cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
           break;
           case 10:
            if(C.getr(1, pkg[1]) == INT)
                C.setr(0, pkg[1], I2);
            else
              cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
           break;
           case 11:
             if(C.getr(1, pkg[1]) == INT)
                C.setr(0, pkg[1], TMP);
             else
              cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
           break;
      }
}

void rmov(double *pkg)
{
     switch( (long) pkg[0] )
     {
          case 0:
             if(pkg[1] == 21)
               EBX = EAX;
            else{
              if(C.getr(1, pkg[1]) == INT)
                 EBX = C.getr(0, pkg[1]);
              else
                cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
            }
           break;
           case 1:
            if(pkg[1] == 21)
               SDX = EAX;
            else{
              if(C.getr(1, pkg[1]) == INT)
                 SDX = C.getr(0, pkg[1]);
              else
                cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
            }
           break;
           case 2:
             if(pkg[1] == 21)
               BP = EAX;
            else{
             if(C.getr(1, pkg[1]) == INT)
               BP = C.getr(0, pkg[1]);
             else
                cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
            }
           break;
           case 3:
	     if(pkg[1] == 21)
               EXC = EAX;
            else{
             if(C.getr(1, pkg[1]) == INT)
                EXC = C.getr(0, pkg[1]);
             else
                cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
            }
           break;
           case 5:
	     if(pkg[1] == 21)
               LG = EAX;
            else{
              if(C.getr(1, pkg[1]) == INT)
               LG = C.getr(0, pkg[1]);
              else
                cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
            }
           break;
           case 6:
             if(pkg[1] == 21)
               LSL = EAX;
            else{
               if(C.getr(1, pkg[1]) == INT)
                   LSL =  C.getr(0, pkg[1]);
               else
                cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
            }
            break;
           case 7:
 	     if(pkg[1] == 21)
               SFC = EAX;
            else{
             if(C.getr(1, pkg[1]) == INT)
               SFC = C.getr(0, pkg[1]);
             else
                cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl; 
           }
           break;
           case 8:
	     if(pkg[1] == 21)
               SCX = EAX;
            else{
              if(C.getr(1, pkg[1]) == INT)
                SCX = C.getr(0, pkg[1]);
              else
                cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
            }
           break;
           case 9:
             if(pkg[1] == 21)
               I1 = EAX;
            else{
             if(C.getr(1, pkg[1]) == INT)
               I1 = C.getr(0, pkg[1]);
	     else
                cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
            }
           break;
           case 10:
             if(pkg[1] == 21)
               I2 = EAX;
            else{
             if(C.getr(1, pkg[1]) == INT)
               I2 = C.getr(0, pkg[1]);
	     else
                cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
            }
           break;
           case 11:
             if(pkg[1] == 21)
               TMP = EAX;
            else{
             if(C.getr(1, pkg[1]) == INT)
                TMP = C.getr(0, pkg[1]); 
             else
                cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
            }
            break;
     }
}

void _init(double *pkg)
{
     switch((long) pkg[0] )
     {
       case 0: // std out
       	 InputOutput io;
        double data[3];
        data[0] = SDX;
        data[1] = 0;
        data[2] = 0;
        Bus b;
        b.accessport(BP);
    //    cout << "port " << BP << " SFC " << SFC << " SCX  " << SCX << endl;
        io.Write(STD_OUT,SFC,SCX,data);
       break;
       case 1: // VHD write
         InputOutput _io;
        double idata[3];
        idata[0] = SDX;
        idata[1] = I1;
        idata[2] = I2;
        Bus buss;
        buss.accessport(BP);
        _io.Write(_VHD,SFC,SCX,idata);
       break;
       case 2: // log
        Log _l;
        if(LG == 1)
          _l.On();
        else if(LG == 0)
          _l.Shutdown();

        SetPriority(LSL);
       break;
       default:
       stringstream ss;
       ss << pkg[0];
       RuntimeException re;
       re.introduce("IllegalSystemCallExcpetion", "code is not a system call [" + ss.str() + "]");
       break;

     }
}

void _port(double *pkg)
{
   Ports p;
     switch( (long) pkg[1] ) {
        case 0:
         if(pkg[0] == 22)
           EAX = p.geto();
         else{
          if(C.getr(1, pkg[0]) == INT)
             C.setr(0, pkg[0], p.geto());
          else
             cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
         }
        break;
        case 1:
           if(pkg[0] == 22)
              p.seto(EAX);
         else{
          if(C.getr(1, pkg[0]) == INT)
             p.seto(C.getr(0, pkg[0]));
          else
             cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
         }
        break;
       case 2:
      Bus b;
         if(pkg[0] == 22)
           EAX = b.status();
         else{
           if(C.getr(1, pkg[0]) == INT)
             C.setr(0, pkg[0], b.status());
           else
             cout << "UnsatisfiedTypeException: warning; type must be of type int" << endl;
         }
         break;
     }
}

Ram r;
void mlock(double *pkg)
{
      for(int i = 0; i < r.info(0); i++){
            if(C.getr(3, i) == 0){
	//	lreg[ i ] = reg[ i ];
                C.setr(2, i, C.getr(0, i));
            }
      }
      for(int i = 0; i < r.info(0); i++){
            if(C.getr(3, i) == 0){
          //      lflag[ i ] = flag[ i ];
                C.setr(4, i, C.getr(1, i));
            }
      }

}

void lock(double *pkg)
{
      if(C.getr(3, pkg[0]) == 0) {
          C.setr(2, pkg[0], C.getr(0, pkg[0]));
          C.setr(2, pkg[0], C.getr(0, pkg[0]));
         // lreg[ pkg[0] ] = reg[ pkg[0] ];
         // lflag[ pkg[0] ] = flag[ pkg[0] ];
      } // else log cannot lock excluded register #{r}
      else {
       stringstream ss;
       ss << pkg[0];
       d_log.v("System","warning Ram: lock err cannot lock excluded address into ram #{" + ss.str() + "}");
       EBX = 2;
      }
}

void xreg(double *pkg)
{
     if(C.getr(3, pkg[0]) == 0)
        C.setr(2, pkg[0], 1);
     else if(C.getr(3, pkg[0]) == 1)
        C.setr(0, pkg[0], 0);
}

void mulock(double *pkg)
{
      for(int i = 0; i < r.info(0); i++){
            if(C.getr(3, i) == 0)
                 C.setr(0, i, C.getr(2, i));
      }

      for(int i = 0; i < r.info(0); i++){
            if(C.getr(3, i) == 0)
                 C.setr(1, i, C.getr(4, i));
      }
}

void clx(double *pkg)
{
     for(int i = 0; i < r.info(0); i++)
           C.setr(3, i, 0);
}

int tibool(bool val)
{
   if(val)
      return 1;
  else
      return 0;
}

void same(double *pkg)
{
       if(C.getr(0, pkg[1]) == 21)
         C.setr(0, pkg[0], tibool(EAX == C.getr(0, pkg[2])));
       else if(C.getr(0, pkg[2]) == 21)
         C.setr(0, pkg[0], tibool(C.getr(0, pkg[1]) == EAX));
       else
         C.setr(0, pkg[0], tibool(C.getr(0, pkg[1]) == C.getr(0, pkg[2])));
}

void ulock(double *pkg)
{
      if(C.getr(3, pkg[0]) == 0) {
         C.setr(0, pkg[0], C.getr(2, pkg[0]));
         C.setr(1, pkg[0], C.getr(4, pkg[0]));
      }
}

void rm(double *pkg)
{
      C.setr(0, pkg[0], null);
      C.setr(1, pkg[0], OI);
}

void func(double *pkg)
{
          if(C.getr(1, pkg[0]) == OI){
            C.setr(0, pkg[0], null);
            C.setr(1, pkg[0], UFUNC);
          }
          else
           cout << "TypeNotSatisfiedException: cannot create func at addr " << pkg[0] << endl;
}

void push(double *pkg)
{
   RuntimeException exception;
       if(C.getr(1, pkg[0]) == UFUNC){
          C.setr(0, pkg[0], IP);
          C.setr(1, pkg[0], FUNC);
          ignore = true;
      }
      else if(C.getr(1, pkg[0]) == FUNC){
            stringstream ss;
            ss << pkg[0];
            exception.introduce("FunctionInitializationException", "multiple definition of func " + ss.str());
     } 
     else
        cout << "NotAFuncException: addr " << pkg[0] << " is not a func" << endl;
}

void _return(double *pkg)
{
        if(ignore)
            ignore = false;
        else {
         if(C.getr(1, pkg[0]) == FUNC)
            IP = (long) C.getr(0, pkg[0]);
         else if(C.getr(1, pkg[0]) == UFUNC)
           cout << "FunctionCallException: cannot return non func addr " << pkg[0] << endl;
         else
           cout << "NotAFuncException: addr " << pkg[0] << " is not a func" << endl;
        }
}

void call(double *pkg)
{
       if(C.getr(1, pkg[0]) == UFUNC) 
             cout << "FunctionCallException: unidentified refrence to func " << pkg[0] << endl;
       else if(C.getr(1, pkg[0]) == FUNC){
          TMP = IP;
          IP = (long) C.getr(0, pkg[0]);
          C.setr(0, pkg[0], TMP);
       }
       else
        cout << "NotAFuncException: addr " << pkg[0] << " is not a func" << endl;
}

void swp(double *pkg)
{
          TMP = C.getr(0, pkg[0]);
          C.setr(0, pkg[0], C.getr(0, pkg[1]));
          C.setr(0, pkg[1], TMP);

         TMP = C.getr(1, pkg[0]);
          C.setr(1, pkg[0], C.getr(1, pkg[1]));
          C.setr(1, pkg[1], TMP);
}

void eof(double *pkg)
{
      C.setr(1, pkg[0], OI);
}

void loop(double *pkg)
{
         C.setr(0, pkg[1], pkg[2]);
         waiting = true;
}

void wloop(double *pkg)
{
      if(C.getr(0, pkg[0]) == 1)
         C.setr(0, pkg[1], IP);
      else {
         pass = true;  // do not run the contents in the loop
         ignore = true;
      }
}

void endwl(double *pkg)
{
  if(pass){
     ignore = false;
     pass = false;
  }
  else if(!ignore && (C.getr(0, pkg[0]) == 1)) // loop again
       IP = C.getr(0, pkg[1]);
}

void rloop(double *pkg)
{
         C.setr(0, pkg[1], C.getr(0, pkg[2]));
         waiting = true;
}

void end() // for do
{
      if(if_ignore){
          if_ignore = false;
          ignore = false;
      }
}

void endl(double *pkg)
{
   if(waiting){
      C.setr(0, pkg[1], (C.getr(0, pkg[1]) - 1));
    if(C.getr(0, pkg[1]) <= 0)
        waiting = false;
   else {
      IP = C.getr(0, pkg[0]);
   }
  }
}

void _do(double *pkg)
{
    if(C.getr(0, pkg[0]) == 1){}
    else {
      if_ignore = true;
      ignore = true;
    }
}

void ilt(double *pkg)
{
    if(C.getr(0, pkg[0]) < C.getr(0, pkg[1])){}
    else {
      if_ignore = true;
      ignore = true;
    }
}

void igt(double *pkg)
{
    if(C.getr(0, pkg[0]) > C.getr(0, pkg[1])){}
    else {
      if_ignore = true;
      ignore = true;
    }
}

void iltoeq(double *pkg)
{
    if(C.getr(0, pkg[0]) <= C.getr(0, pkg[1])){}
    else {
      if_ignore = true;
      ignore = true;
    }
}

void igtoeq(double *pkg)
{
    if(C.getr(0, pkg[0]) >= C.getr(0, pkg[1])){}
    else {
      if_ignore = true;
      ignore = true;
    }
}

void ndo(double *pkg)
{
    if(C.getr(0, pkg[0]) == 0){}
    else {
      if_ignore = true;
      ignore = true;
    }
}

void inlt(double *pkg)
{
    if(!(C.getr(0, pkg[0]) < C.getr(0, pkg[1]))){}
    else {
      if_ignore = true;
      ignore = true;
    }
}

void ingt(double *pkg)
{
    if(!(C.getr(0, pkg[0]) > C.getr(0, pkg[1]))){}
    else {
      if_ignore = true;
      ignore = true;
    }
}

void inltoeq(double *pkg)
{
    if(!(C.getr(0, pkg[0]) <= C.getr(0, pkg[1]))){}
    else {
      if_ignore = true;
      ignore = true;
    }
}

void ingtoeq(double *pkg)
{
    if((C.getr(0, pkg[0]) >= C.getr(0, pkg[1])) == false){}
    else {
      if_ignore = true;
      ignore = true;
    }
}

void neg(double *pkg){
    if(C.getr(1, pkg[0]) == INT || C.getr(1, pkg[0]) == DOUBLE)
      C.setr(0, pkg[0], (C.getr(0, pkg[0]) * -1));
    else
      cout << "TypeNotatisfiedException: only types of int and double can be converted to negative" << endl;
}
