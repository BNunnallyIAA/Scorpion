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
* Scorpion Virtural Machine
* Designed and developed by Braxton Nunnally
*
*/

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

// ------[ Header Variables ]------
// the magic number required at the beginning of every Scorpion Application
const char *magic = (char 0xCA) +"" + (char 0xDB) + "" (char 0xFE) + "" + (0xAO);
const char offsetkey = 0x0;
const char headerbeginkey = 0xAA, headerendkey = 0xAF;
const char *format_version = "0.2.1.0";
const char *minor_format_vers = "0.2";

// ------{ Header Flag keys }-------
const char fv = 0xBB;
const char mfv = 0xBA;
const char targetdv = 0xBC, mindv = 0xBE; // the target and minimum dev version allowed

// ------{ Img Flag Keys }------
const char str = 0xBF; // string data ahead
const char strstart = '^';
//[str][size][strstart][------][offsetkey]

/*
* dev_version
* This is a numeric value representing the 
* current build version the Scorpion Vurtual Machine 
* is on. 
*
* This will mainly be used for developers who only want 
* an application to target specific builds that are allowed 
* to run on the scorpion Virtual machine.
*/
const int dev_version = 7;

short header = 0;

bool hasmagic(const char *m)
{
   if(m == magic)
	   return true;
   else return false;
}

bool isheaderkey(const char key)
{
	if((key == headerbeginkey) || (key == headerendkey)){
		header++;
	    return true;
	}
  return false;
}

short getheaderstatus()
{
	return header;
}

int getdev_version()
{
	return dev_version;
}

bool isstrkey(const char key)
{
	if(key == str)
		return true;
	return false;
}

bool isstr_startkey(const char key)
{
	if(key == strstart)
		return true;
	return false;
}

const char* getflag(const char key)
{
	if(key == fv)
		return "format version";
	else if(key == mfv)
		return "minor format version";
	else if(key == targetdv)
		return "target dev version";
	else if(key == mindv)
		return "minor dev version";
	else
		return "?";
}