#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

public namespace Heap {

      #define HEAP_DIVISOR 3
      long heapSize, Xmax, Xmin;

      long actual(){ // get actual heap size allocated per each pool
         return heapSize / HEAP_DIVISOR;
      }

      class Stack {
          long size_t; // the stack size (unchangeable!)
          long *ret, *registers;
          double *local;

          public:
             MethodPool mPool;
             DataPool mDataPool;
             long size(Stack s);
      };

      long Stack::size(Stack mStack){
          return mStack.size_t;
      }

      class MethodPool {
           long size_t; // the size of the allocated memory
           long *method; // Both the main and secondary memory for ram

            public:
              long size(MethodPool mPool); // get the size in memory
      };

      long MethodPool::size(MethodPool mPool){
                return MethodPool.size_t;
      }

      class DataPool {
          long size_t; // the size of the allocated memory
          double *main, *secondary; // Both the main and secondary memory for ram
          stringstream image; // the current running application image

          public:
              long size(DataPool mDataPool); // get the size in memory
      };

       long DataPool::size(DataPool mDataPool){
          return MethodPool.size_t;
       }

      class TraceBack {

      };
}

