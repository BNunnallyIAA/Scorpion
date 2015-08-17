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
const char vernum = 0xCB, debg = 0xCC, 
	       lgg = 0xCD, lgpd = 0xCE, lgfle = 0xCF;

// ------{ Img Flag Keys }------
const char str = 0xBF; // string data ahead
const char strstart = '^';
//[str][size][strstart][------][offsetkey]

/*
* dev_version
* This is a numeric value representing the 
* current build version the Scorpion Virtual Machine 
* is on. 
*
* This will mainly be used for developers who only want 
* an application to target specific builds that are allowed 
* to run on the scorpion Virtual machine.
*/
const int dev_version = 7;

short header = 0;
int index = 0,  imgsize; // where we are in the file
stringstream image; // the image code to be executed
stringstream spk; // the actual application file saved in memory

bool hasmagic(const char *m)
{
   if(m == magic)
	   return true;
   else return false;
}

bool isheaderkey(const char key)
{
	if(key == headerbeginkey)
	    return true;
  return false;
}

bool isheaderclosekey(const char key)
{
	if(key == headerendkey)
	    return true;
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
	else if(key == vernum)
		return "version number";
	else if(key == debg)
		return "debug";
	else if(key == lgg)
		return "log";
	else if(key == lgpd)
		return "log precedence";
	else if(key == lgfle)
		return "log file";
	else
		return "?";
}

char nextchar()
{
	return spk.str().at(index++);
}

const char *getmagic()
{
	stringstream mg;
	mg << nextchar() << nextchar() 
	   << nextchar() << nextchar();
	return mg.str().c_str();
}

void jumptoheader()
{
	for(int i = 0; i < spk.str().length(); i++)
	{
		if(nextchar() == headerbeginkey)
			return;
	}
}

void eatnull()
{
	for(int i = 0; i < spk.str().length(); i++)
	{
		if(nextchar() != offsetkey)
			return;
	}
}

void readheader()
{
	for(int i = 0; 
}

int parse(string _spk)
{
	spk << _spk;
	if(hasmagic(getmagic()))
	{
		nextchar(); // eat offset char
		jumptoheader();
		readheader();
	}
	else{
		cout << "failure magic number not found" << endl;
	}
}

