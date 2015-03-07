/**
* Virtural Random Access Memory Chip Designed by Braxton Nunnally
*
* Cells: 5
* Model: DM3 101
* Production Date: 3-15
* R\C: 1mb
* Max Size: 5mb
*
* The DM3 101 Ram chip features a heavy 5 cell access ratio
* and a tiny ammount of Ram w/ status reports of access status
*
*
* For full info visit "https://github.com/AndroDevcd/RPVM/blob/master/LINEUP.md"
*/

#include "../CPU/core0/core0.h"
#include "../program.h"
#include "ram.h"
#include <iostream>
using namespace std;

#define MAX 1000000 /* 1mb of R\c(ram per cell)*/
#define NUM_CELLS 4
double ram[ MAX ]; // cell 0
double fram[ MAX ]; // cell 1
double lram[ MAX ];// cell 2
double xram[ MAX ];// cell 3
double lfram[ MAX ];// cell 4

int INDEX_OUT_OF_RANGE = 0x0025;
int INDEX_OK = 0x0026;
int rsize;

   int state;

long Ram::CB = 0;
long address = 0; 
int Ram::addr(long index)
{
  if(index < 0 || index > MAX){
         state = INDEX_OUT_OF_RANGE;
     return INDEX_OUT_OF_RANGE;
  }
  else
     address = index;
  state = INDEX_OK;
  return INDEX_OK;
}

double Ram::data(int cell ,double dataBus)
{
  if( state == INDEX_OK ){
   if(cell == 0) {
      switch( Ram::CB ) {
         case 1: // S
        //   cout << "Saving " << dataBus<< "to ram CB " << Ram::CB << " address " << address << endl;
           ram[ address ] = dataBus;
         break;
         case 2: // E
        //   cout << "Getting " << dataBus<< "from ram CB " << Ram::CB << " address " << address << endl;
           return ram[ address ];
         break;
         default:
           cout << "Ram: control_bus_failure err" << endl;
           EBX = 1;
           p_exit();
         break;

      }
   }
   else if(cell == 1){
      switch( Ram::CB ) {
         case 1: // S
           fram[ address ] = dataBus;
         break;
         case 2: // E
           return fram[ address ];
         break;
         default:
           cout << "Ram: control_bus_failure err" << endl;
           EBX = 1;
           p_exit();
         break;

      }
   }
   else if(cell == 2){
      switch( Ram::CB ) {
         case 1: // S
           lram[ address ] = dataBus;
         break;
         case 2: // E
           return lram[ address ];
         break;
         default:
           cout << "Ram: control_bus_failure err" << endl;
           EBX = 1;
           p_exit();
         break;

      }
   }
   else if(cell == 3){
      switch( Ram::CB ) {
         case 1: // S
           xram[ address ] = dataBus;
         break;
         case 2: // E
           return xram[ address ];
         break;
         default:
           cout << "Ram: control_bus_failure err" << endl;
           EBX = 1;
           p_exit();
         break;

      }
   }
    else if(cell == 4){
      switch( Ram::CB ) {
         case 1: // S
           lfram[ address ] = dataBus;
         break;
         case 2: // E
           return lfram[ address ];
         break;
         default:
           cout << "Ram: control_bus_failure err" << endl;
           EBX = 1;
           p_exit();
         break;

      }
   }
   else{
     cout << "Ram: warning cell_not_found err" << endl;
     cout << "Ram: reverting to cell 0" << endl;
      switch( Ram::CB ) {
         case 1: // S
           ram[ address ] = dataBus;
         break;
         case 2: // E
           return ram[ address ];
         break;
         default:
           cout << "Ram: control_bus_failure err" << endl;
           EBX = 1;
           p_exit();
         break;
      }
   }
  }
  else if(state == INDEX_OUT_OF_RANGE) {
     cout << "Ram: index_out_of_range err" << endl;
     EBX = 1;
     p_exit();
  }
  else 
    cout << "Ram: state_not_known err" << endl;
  state = 0; // dump addr state
  Ram::CB = 0; // dump the control bus
  address = 0; // dump address
  return 0;
}

long b_tmb(long bytes)
{
   return (bytes / 1000000);
} // convert from bytes to mb

long Ram::info(int info)
{
   if(info == 0)
    return MAX; // return r/c unformatted
   else if(info == 3)
    return b_tmb(MAX); // return r/c formatted
   else if(info == 1)
    return b_tmb(MAX * NUM_CELLS); // return total ram size
   else if(info == 2)
    return NUM_CELLS; // return the total num of ram cells
  else 
    return 0;
}