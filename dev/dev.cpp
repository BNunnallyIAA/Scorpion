#include <string>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

using namespace std;

int index = 0;


string dev = "{ # Application\n   name 'TestApplication'        # Manditory\n   {\n       minDevVersion 7           # Manditory\n   \t   TargetDevVersion 7        # Manditory\n\t   versionNumber '0.0.7'     # Manditory\n   }\n   \n   { # Singing configs\n       debug false \n   }\n\n   { # Logging\n       log true\n\t   logPrecedence 7\n\t   logFile ('file.log') # output file\n   }\n}";

bool eof = false;
string nextLine()
{
   stringstream str;
   str << "";
   for(int i = index; i < dev.length(); i++)
   {
     if((dev.at(i) == '\n') || (dev.at(i) == 10)){
       index++;
       return str.str();
     }
     else str << dev.at(i);
     if(!((i + 1) < dev.length())){
       index++;
       return str.str();
     }
     index++;
   }
  eof = true;
  return "--?";
}

bool iswhitespace(char c)
{
   if((c == '\t') || (c == ' ') || (c == '\n'))
     return true;
  return false;
}

bool isbreaker(char c)
{
   if(c == '{' || c == '}')
     return true;
  return false; 
}

bool iscomment(char c)
{
   if(c == '#')
     return true;
  return false;
}

string lexedChars[100];
int charindex = 0, charsize = 0;
int scope = 0;

void lex(string line)
{
  line.append(" \n");
  stringstream chars;
  for(int i = 0; i < line.length(); i++)
  {
    if(iswhitespace(line.at(i)) || isbreaker(line.at(i))){
      if(line.at(i) == '{')
        scope++;
      else if(line.at(i) == '}')
        scope--;
      if(chars.str() != ""){
         lexedChars[ charindex++ ] = chars.str();
         charsize++;
         chars.str("");
      }
    }
    else if(iscomment(line.at(i)))
      break;
    else
     chars << "" << line.at(i);
  }

}

string getchars()
{
   stringstream ss;
   for(int i = 0; i < charsize; i++)
   {
      ss << ", " << lexedChars[i] << " ";
   }
  return ss.str();
}

bool isodd(int x)
{
  if((x % 2) != 0)
   return true;
  else
  return false;
}

string attribs[9] { "name", "minDevVersion", "targetDevVersion", "versionNumber",
                   "debug", "log", "logPrecedence", "logFile" };

bool hasstring(string data)
{
   for(int i = 0; i < sizeof(attribs); i++)
   {
     if(data == attribs[i])
       return true;
   }
  return false;
}

bool isattribute(string attb)
{
   if(hasstring(attb))
    return true;
  return false;
}

string specificarry[5];

// The different Tag types
int TYPE_STRING = 1;
int TYPE_NUMBER = 2;
int TYPE_SPECIFIC = 3;

int ETYPE = 0;

void flush()
{
   specificarry[0] = "";
   specificarry[1] = "";
   specificarry[2] = "";
   specificarry[3] = "";
   specificarry[4] = "";
}

void settag_type(string attrib)
{
   if(attrib == attribs[0]) // name
    ETYPE = TYPE_STRING;
   else if(attrib == attribs[1]) // minDevVersion
    ETYPE = TYPE_NUMBER;
   else if(attrib == attribs[2]) // targetDevVersion
    ETYPE = TYPE_NUMBER;
   else if(attrib == attribs[3]) // versionNumber
    ETYPE = TYPE_STRING;
   else if(attrib == attribs[4]){ // debug
    ETYPE = TYPE_SPECIFIC;
    flush();
    specificarry[0] = "true";
    specificarry[1] = "false";
   }
   else if(attrib == attribs[5]){ // log
    ETYPE = TYPE_SPECIFIC;
    flush();
    specificarry[0] = "true";
    specificarry[1] = "false";
   }
   else if(attrib == attribs[6]) // logPrecedence
    ETYPE = TYPE_NUMBER;
   else if(attrib == attribs[7]) // logFile
    ETYPE = TYPE_STRING;
}

bool isstring(string data)
{
   if((data.at(0) == '\'') && (data.at(data.length() -1) == '\''))
     return true;
   else if(((data.at(0) == '(') && (data.at(data.length() -1) == ')')) &&
           ((data.at(1) == '\'') && (data.at(data.length() -2) == '\'')))
     return true;
  return false;
}

bool isdigit(char c)
{
  if(c == '0')
    return true;
  else if(c == '1')
    return true;
  else if(c == '2')
    return true;
  else if(c == '3')
    return true;
  else if(c == '4')
    return true;
  else if(c == '5')
    return true;
  else if(c == '6')
    return true;
  else if(c == '7')
    return true;
  else if(c == '8')
    return true;
  else if(c == '9')
    return true;
  else
    return false;
}

bool isnumber(string data)
{
  for(int i = 0; i < data.length(); i++)
  {
    if(!isdigit(data.at(i)))
     return false;
  }
  return true;
}


bool isspecific(string data)
{
  for(int i = 0; i < 5; i++)
  {
     if(data == specificarry[i])
      return true;
  }
  return false;
}

int gettype(string data)
{
   if(isstring(data))
     return TYPE_STRING;
   else if (isnumber(data))
     return TYPE_NUMBER;
   else if(isspecific(data))
     return TYPE_SPECIFIC;
   else
     return -1;
}

bool isvalid_tag(string tg)
{
  if(ETYPE == gettype(tg))
   return true;
 return false;
}

string attrubute = "", tag = "";
void process()
{
  if(charsize == 0)
   return;
  else {
    if(charsize > 2){
       cout << " attribute: " << lexedChars[0] << " has too many tags!" << endl;
    }
    attrubute = lexedChars[0];
    tag = lexedChars[1];

    if(isattribute(attrubute)){
      cout << "setting" << endl;
       settag_type(attrubute);
      cout << "checking" << endl;
       if(isvalid_tag(tag)){
          cout << "the attribute:" << attrubute << " has a valid tag" << endl;
       }
       else
        cout << "the attribute:" << attrubute << " does not have a valid tag" << endl;
    }
    else{
     cout << "the attribute:" << attrubute << " is not a know attrubute";
    }
  }
}

int linenum = 0;
int main()
{
  cout << "flushing" << endl;
  flush();
  while(!eof)
  {
    linenum++;
    stringstream line;
    line << nextLine();
  if(line.str() != "--?"){
    lex(line.str());
    printf("line: %d charsize: %d [ %s ]\n", linenum, charsize, getchars().c_str());
    process();
    charindex = 0;
    charsize = 0;
//      cout << "line: " << line.str() << endl;
    }
//    else
//     return 0;
  }
  if( scope > 0 )
   cout << "missing } at end of file\n";
  else if( scope < 0 )
   cout << "unexpected char '}' at end of file" << endl;
  else { }
  cout << "scope: " << scope;
  return 0;
}
