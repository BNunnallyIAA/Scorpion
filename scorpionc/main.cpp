/*
* Scorpion Compiler revision 6
* version 0.2.0
* build date: July 2015
*
* This compiler is created based
* on the most recent revision of the
* Scorpion programming language.
*
* For up-to-date documentation the the
* most recent Revision of Scorpion, please
* visit "https://github.com/AndroDevcd/Scorpion/wiki"
*
*
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
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct FileFormat {
  char magic_number[4];
  char headerBegin, headerEnd;
  short date_created[4];
  short format_version[3];
  long img_size;
  stringstream image;
} ;

FileFormat fileFormat;

struct VariablePool {
  string name = "";
  int access_flag = 0;
  int data_type = 0;
  long addr = 0;
} ;

VariablePool variablePool;

struct MethodPool {
  string name = "";
  int access_flag = 0;
  int return_type = 0;
  long addr = 0;
  int agr_size = 0;
  const char *args;
} ;

MethodPool methodPool;

struct ClassMap {
  variablePool vpool[ 1000 ];
  methodPool mpool[ 1000 ];
  /* Class Info */
  string name = "";
  int methodSize;
  int variableSize;
  int accessFlag;
} ;

ClassMap classMap;

int main()
{
  /* Setup Magic Number */
  fileFormat.magic_number[0] = 0xCA;
  fileFormat.magic_number[1] = 0xDB;
  fileFormat.magic_number[2] = 0xFE;
  fileFormat.magic_number[3] = 0xAO;

  /* Setup Header Indicators */
  fileFormat.headerBegin = 0xAA;
  fileFormat.headerEnd = 0xAF;
  
  fileFormat.format_version[0] = 0x0;
  fileFormat.format_version[1] = 0x1;
  fileFormat.format_version[2] = 0x0;
  return 0;
}
