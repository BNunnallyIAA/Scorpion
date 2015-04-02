#include "core0.h"
#include "cpuf.h"
#include "runtime_exception.h"
#include <sstream>

C0 c;
long _xor(int,int);
long _or(int,int);
long _and(int,int);
long _not(int);
long _nand(int,int);
long _nor(int,int);
long _xnor(int,int);

long _xor(int a, int b)
{
    if((_and(_not(a),b)) || (_and(a,_not(b))))
       return 1;
    else
       return 0;
}

long _or(int a, int b)
{
    if((a == 1) || (b == 1))
      return 1;
    else
      return 0;
}

long _and(int a, int b)
{
  if(a == b)
     return 1;
  else
     return 0;
}

long _not(int a)
{
   if(a == 1)
      return 0;
   else
      return 1;
}


long _nand(int a, int b)
{
    if(_and(a,b))
        return 0;
    else
        return 1;
}

long _nor(int a,int b)
{
   if(_or(a,b))
       return 0;
   else
       return 1;
}

long _xnor(int a, int b)
{
   if(_xor(a,b))
      return 0;
   else
      return 1;
}

int ibool(long num)
{
     if((num == 1) || (num == 0))
        return num;
     else if(num > 1)
        return 1;
     else
        return 0;
}


void nand_l(double *pkg)
{
  RuntimeException re;
   if(pkg[0] == 21)
       EAX = _nand(ibool(pkg[1]),ibool(pkg[2]));
   else
   {
      if( c.getr(1, pkg[0]) == BOOL )
           c.setr(0, pkg[0], _nand(ibool(c.getr(0, pkg[1])),ibool(c.getr(0, pkg[2]))));
      else
        re.introduce("UnsatisfiedTypeException","the type reciving the input must be of type bool");
   }
}

void nor_l(double *pkg)
{
  RuntimeException re;
   if(pkg[0] == 21)
     EAX = _nor(ibool(pkg[1]),ibool(pkg[2]));
   else
   {
      if( c.getr(1, pkg[0]) == BOOL )
           c.setr(0, pkg[0], _nor(ibool(c.getr(0, pkg[1])),ibool(c.getr(0, pkg[2]))));
      else
        re.introduce("UnsatisfiedTypeException","the type reciving the input must be of type bool");
   }
}

void xnor_l(double *pkg)
{
  RuntimeException re;
   if(pkg[0] == 21)
       EAX = _xnor(ibool(pkg[1]),ibool(pkg[2]));
   else 
   {
      if( c.getr(1, pkg[0]) == BOOL )
           c.setr(0, pkg[0], _xnor(ibool(c.getr(0, pkg[1])),ibool(c.getr(0, pkg[2]))));
      else
        re.introduce("UnsatisfiedTypeException","the type reciving the input must be of type bool");
   }
}

void and_l(double *pkg)
{
  RuntimeException re;
   if(pkg[0] == 21)
       EAX = _and(ibool(pkg[1]),ibool(pkg[2]));
   else
   {
      if( c.getr(1, pkg[0]) == BOOL )
           c.setr(0, pkg[0], _and(ibool(c.getr(0, pkg[1])),ibool(c.getr(0, pkg[2]))));
      else
        re.introduce("UnsatisfiedTypeException","the type reciving the input must be of type bool");
   }
}

void or_l(double *pkg)
{
  RuntimeException re;
   if(pkg[0] == 21)
     EAX = _or(ibool(pkg[1]),ibool(pkg[2]));
   else
   {
      if( c.getr(1, pkg[0]) == BOOL )
          c.setr(0, pkg[0], _or(ibool(c.getr(0, pkg[1])),ibool(c.getr(0, pkg[2]))));
      else
        re.introduce("UnsatisfiedTypeException","the type reciving the input must be of type bool");
   }
}

void xor_l(double *pkg)
{
  RuntimeException re;
   if(pkg[0] == 21)
       EAX = _xor(ibool(pkg[1]),ibool(pkg[2]));
   else 
   {
      if( c.getr(1, pkg[0]) == BOOL )
           c.setr(0, pkg[0], _xor(ibool(c.getr(0, pkg[1])),ibool(c.getr(0, pkg[2]))));
      else
        re.introduce("UnsatisfiedTypeException","the type reciving the input must be of type bool");
   }
}

void not_l(double *pkg)
{
  RuntimeException re;
     if(pkg[0] == 21)
       EAX = _not(ibool(pkg[1]));
   else
   {
      if( c.getr(1, pkg[0]) == BOOL )
            c.setr(0, pkg[0], _not(ibool(c.getr(0, pkg[1]))));
      else
        re.introduce("UnsatisfiedTypeException","the type reciving the input must be of type bool");
   }
}