#include <string>
#include <algorithm>
#include <iostream>
using namespace std;
using namespace Heap, Thread;

namespace Alloc {

      int ALLOC_SUCCESSFUL = 0x0;
      int ALLOC_OUT_OF_MEMORY = 0xFE;
      int ALLOC_FAILED = 0xAF;
      int ALLOC_STATUS_UNKNOWN = 0xFF;
      int ALLOC_SIZE_INVALID = 0xCB;


      #define nullptr ((void *)0)

     /*
     * Init() initially setup stack
     */
     int scorpion_Init(long stackSize, long memory, string name, int threadId, long Xmax, long Xmin, string image){
           // HEAP SETUP
           // -----------------------------
           Heap::Xmax = Xmax;
           Heam::XMin = Xmin;
           Heap::heapSize = memory;
           // Create the stack
           if(stackSize <= 0)
              return ALLOC_SIZE_INVALID;

           Heap::Stack scorpion_heap_stackInit_ = new Heap::Stack();
           scorpion_heap_stackInit_.size_t = stackSize;
           scorpion_heap_stackInit_.ret = new (nothrow) long[stackSize];

           if(scorpion_heap_stackInit_.ret == nullptr)
               return ALLOC_OUT_OF_MEMORY;
           scorpion_heap_stackInit_.registers = new long[20];
           scorpion_heap_stackInit_.local = new (nothrow) double[stackSize];

           if(scorpion_heap_stackInit_.local == nullptr)
               return ALLOC_OUT_OF_MEMORY;

           // Setup Data pools
           scorpion_heap_stackInit_.mPool=new MethodPool();
           scorpion_heap_stackInit_.mPool.size_t = memory;
           scorpion_heap_stackInit_.mPool.method = new (nothrow) long[memory];

            if(scorpion_heap_stackInit_.mPool.method == nullptr)
                return ALLOC_OUT_OF_MEMORY;

           scorpion_heap_stackInit_.mDataPool=new DataPool();

            if(scorpion_heap_stackInit_.mDataPool.main == nullptr)
                 return ALLOC_OUT_OF_MEMORY;
             if(scorpion_heap_stackInit_.mDataPool.secondary == nullptr)
                 return ALLOC_OUT_OF_MEMORY;
           // -----------------------------

           if(Thread::newThread(name, threadId) != 0)
               return ALLOC_FAILED;

           Thread::setStack(scorpion_heap_stackInit_);
           Thread::setSource(image);

           if(Thread::setActiveThread(threadId) != 0)
               return ALLOC_FAILED;

        return ALLOC_SUCCESSFUL;
     }
}
