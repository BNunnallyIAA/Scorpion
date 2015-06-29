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
*/
#include "io.h"
#include "core0.h"
#include "../../GPIO/gpio.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

extern string p;
extern void tostring(const char *name);
extern bool file_exists(const char *file);
int ibool(long);

string file_name = "";
std::ofstream stream;
long InputOutput::Write(int device,  long *data)
{
   switch( device ) {
      case 0:  // GPIO
        return GPIOWrite(SDX, SCX);
      break;
      case 1:
       {
         if(data[0] == 1) // open a file
         {
            if(file_exists(file_name.c_str())){
               const char *f = file_name.c_str();
               stream.open(f);
               if (stream.is_open())
                 return 1;
               else
                 return 0;
            }
            else
             return -1;
         }
         else if(data[0] == 2)
         {
           if (stream.is_open())
              stream << core0.buf.str();
           else
            return -1;
         }
         else if(data[0] == 3)
         {
           if (stream.is_open())
             stream.close();
           else
            return -1;
         }
         else
          return -3;
       }
      break;
  }
   return 0;
}

long InputOutput::Read(int device,  long *data)
{
   switch( device ) {
      case 0:  // GPIO
        return GPIORead(SDX);
      break;
      case 1: // File IO
       {
           if(data[0] == 1) // check for a file's presence
          {
             TMP = SDX;
             file_name = "";
             for(int i = TMP + 1; i < (core0.getr(0, SDX) + SDX) + 1; i++)
                 file_name += core0.getr(0, i);
             return ibool(file_exists(file_name.c_str()));
          }
          else if(data[0] == 6) // load the temporary file data to the ram
          {
             TMP = SDX + 1;
             int ch;
             string data = core0.buf.str();
             for(long i = 0; i < p.length(); i++)
             {
                  ch = data.at(i);
                  core0.setr(0, TMP++, ch);
             }

          }
          else if(data[0] == 2) // save the file in a temporary location
          {
             if(file_exists(file_name.c_str())){
                 const char *f = file_name.c_str();
                  string tmp;
                  core0.buf.str(""); // flush out input buffer
		  ifstream input(f);
                  long length = 0;

     		  while(!input.eof()) {
         	    tmp = "";
         	    getline(input, tmp);
                    core0.buf << tmp;
                    core0.buf << "\n";
                  }

                 core0.setr(0, SDX, core0.buf.str().size());
             }
             else
               return -1;
          }
          else
           return -3;
       }
      break;
  }
   return 1;
}

