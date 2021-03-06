#include <string>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int index = 0;

string dev = "{ # Application\n    name 'TestApplication' # Manditory\n   {\n          minDevVersion 7   # Manditory\n   \t   targetDevVersion 7        # Manditory\n\t      versionNumber '0.0.2.68'  # Manditory\n   }\n   \n   { # Singing configs\n       debug false \n   }\n\n   { # Logging\n       log true\n\t   logPrecedence 7\n\t   logFile ('file.log') # output file\n   }\n}";

//string dev = "if( scope > 0 ) \ncout << missing } at end of file\n;\nelse if( scope < 0 )\ncout << unexpected char '}' at end of file vhdisfv nnshbdsj  vhsdsbfbdshjfkjnsdpoaiuweoph9fw8ygij yuds cxvefcvc bfdsvfyhj hx ivewhj iuvw << endl;\nelse { }\ncout << scope:  << scope\nreturn 0;";

// Attributes
string name = "TestApplication";
int minDevVersion = 7;
int targetDevVersion = 7;
string version_number = "0.0.1.0";
bool debug = false, loog = true;
int logprecedence = 7;
string logfile = "/usr/share/scorpion/log.txt";

int response = 0;
int linenum = 0;
string file_name = "build.dev";

// ------------------------ Attribute boolean
bool nme = false, mdv = false, tdv = false, 
     vn = false, db = false, lp = false, lg = false, lf = false;


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
bool instring = false;

void lex(string line)
{
  line.append(" \n");
  stringstream chars;
  for(int i = 0; i < line.length(); i++)
  {
    if(line.at(i) == '\''){
        if(instring)
          instring = false;
        else
          instring = true;
    }
    
    if((iswhitespace(line.at(i)) || isbreaker(line.at(i))) && !instring){
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
    else if(iscomment(line.at(i))){
      if(chars.str() != ""){
         lexedChars[ charindex++ ] = chars.str();
         charsize++;
         chars.str("");
      }
      break;
    }
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

string attribs[8] = { "name", "minDevVersion", "targetDevVersion", "versionNumber",
                   "debug", "log", "logPrecedence", "logFile" };

stringstream errormsg, warningmsg;
int warnings, errors;

void deverror(string message)
{
    response = -1;
	errors++;
    errormsg << "build error: " << file_name  << ":" << linenum << "  " << message << endl;
}

void devwarning(string message)
{
	warnings++;
    warningmsg << "warning: " << file_name  << ":" << linenum << "  " << message << endl;
}

bool hasstring(string attrib)
{
   if(attrib == attribs[0]) // name
      return true;
   else if(attrib == attribs[1]) // minDevVersion
      return true;
   else if(attrib == attribs[2]) // targetDevVersion
      return true;
   else if(attrib == attribs[3]) // versionNumber
      return true;
   else if(attrib == attribs[4]) // debug
      return true;
   else if(attrib == attribs[5]) // log
      return true;
   else if(attrib == attribs[6]) // logPrecedence
      return true;
   else if(attrib == attribs[7]) // logFile
      return true;
   else
  return false;
}

bool isattribute(string attb)
{
   if(hasstring(attb))
    return true;
  return false;
}

string var1, var2;

// The different Tag types
int TYPE_STRING = 1;
int TYPE_NUMBER = 2;
int TYPE_SPECIFIC = 3;

int ETYPE = 0;

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
    var1 = "true";
    var2 = "false";
   }
   else if(attrib == attribs[5]){ // log
    ETYPE = TYPE_SPECIFIC;
    var1 = "true";
    var2 = "false";
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
  if(data == var1 || data == var2)
     return true;
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

string getstring(string data)
{
    if(data.at(0) == '('){
       stringstream ss;
       for(int i = 2; i < data.length() - 2; i++)
          ss << data.at(i);
       return ss.str();
    }
    else{
        stringstream ss;
       for(int i = 1; i < data.length() - 1; i++)
          ss << data.at(i);
       return ss.str();
    }
}

int getint(string data)
{
   return atoi(data.c_str());   
}

bool getbool(string data)
{
    if(data == "true")
      return true;
    else
      return false;
}

void updateattrib(string attrib, string tag)
{
    if(attrib == attribs[0]){ // name
    if(nme == true){
       stringstream ss;
       ss << "attribute:" << attrib << " has already been set.";
       devwarning(ss.str());
    }
    else
      cout<< "   build:" << attrib << "=UPDATED" << endl;
       name = getstring(tag);
       nme = true;
    }       
   else if(attrib == attribs[1]){ // minDevVersion
    if(mdv == true){
       stringstream ss;
       ss << "attribute:" << attrib << " has already been set.";
       devwarning(ss.str());
    }
    else
      cout<< "   build:" << attrib << "=UPDATED" << endl;
       minDevVersion = getint(tag);
       mdv = true;
   }
   else if(attrib == attribs[2]){ // targetDevVersion
    if(tdv == true){
       stringstream ss;
       ss << "attribute:" << attrib << " has already been set.";
       devwarning(ss.str());
    }
    else
      cout<< "   build:" << attrib << "=UPDATED" << endl;
       targetDevVersion = getint(tag);
       tdv = true;
   }
   else if(attrib == attribs[3]){ // versionNumber
    if(vn == true){
       stringstream ss;
       ss << "attribute:" << attrib << " has already been set.";
       devwarning(ss.str());
    }
    else
      cout<< "   build:" << attrib << "=UPDATED" << endl;
       version_number = getstring(tag);
       vn = true;
   }
   else if(attrib == attribs[4]){ // debug
    if(db == true){
       stringstream ss;
       ss << "attribute:" << attrib << " has already been set.";
       devwarning(ss.str());
    }
    else
      cout<< "   build:" << attrib << "=UPDATED" << endl;
       debug = getbool(tag);
       db = true;
   }
   else if(attrib == attribs[5]){ // log
    if(lg == true){
       stringstream ss;
       ss << "attribute:" << attrib << " has already been set.";
       devwarning(ss.str());
    }
    else
      cout<< "   build:" << attrib << "=UPDATED" << endl;
       loog = getbool(tag);
       lg = true;
   }
   else if(attrib == attribs[6]){ // logPrecedence
    if(lp == true){
       stringstream ss;
       ss << "attribute:" << attrib << " has already been set.";
       devwarning(ss.str());
    }
    else
      cout<< "   build:" << attrib << "=UPDATED" << endl;
       logprecedence = getint(tag);
       lp = true;
   }
   else if(attrib == attribs[7]){ // logFile
    if(lf == true){
       stringstream ss;
       ss << "attribute:" << attrib << " has already been set.";
       devwarning(ss.str());
    }
    else
      cout<< "   build:" << attrib << "=UPDATED" << endl;
       logfile = getstring(tag);
       lf = true; 
   }
}

string attrubute = "", tag = "";
void process()
{
  if(charsize == 0)
   return;
  else {
    if(charsize > 2){
       stringstream ss;
       ss << "attribute:" << lexedChars[0] << " has too many tags!";
       deverror(ss.str());
	   return;
    }
    attrubute = lexedChars[0];
    tag = lexedChars[1];

    if(isattribute(attrubute)){
       settag_type(attrubute);
       if(isvalid_tag(tag)){
          updateattrib(attrubute, tag);
       }
       else{
        stringstream ss;
        ss << "attribute:" << attrubute << " tag is invalid";
        deverror(ss.str());
       }
    }
    else{
      stringstream ss;
      ss << "attribute:" << attrubute << " is not a know attrubute";
      deverror(ss.str());
    }
  }
}

void printwarnings()
{
	cout << "WARNINGS " << warnings << endl;
	cout << warningmsg.str() << endl;
}

void printerrors()
{
	cout << "ERRORS " << errors << endl;
	cout << errormsg.str() << endl;
}

int count = 0;
string format(string data)
{
    if(count == 0){
        count++;
        stringstream ss;
        ss << data << " ";
        return ss.str();
    }
    else {
       count++;
       stringstream ss;
       ss << " : " << data << " ";
       return ss.str();
    }
}

void checkattribs()
{
   stringstream ss;
   ss << "";
   if(!nme)
      ss << format("name");
   if(!mdv)
      ss << format("minDevVersion");
   if(!tdv)
      ss << format("targetDevVersion");
   if(!vn)
      ss << format("versionNumber");
      
   if(ss.str() != ""){
       stringstream sm;
       sm << "missing required attribute(s): [ " << ss.str() << "]";
       deverror(sm.str());
   }
}

int main()
{
  cout << "Dev Build running..." << endl;
  while(!eof)
  {
    linenum++;
    stringstream line;
    line << nextLine();
  if(line.str() != "--?"){
    lex(line.str());
//    printf("line: %d charsize: %d [ %s ]\n", linenum, charsize, getchars().c_str());
    process();
    charindex = 0;
    charsize = 0;
    }
   else
     break;
  }
  
  if( scope > 0 ){
     stringstream ss;
     ss << "missing '}' at end of file!";
     deverror(ss.str());
  }
  else if( scope < 0 ){
     stringstream ss;
     ss << "unexpected char '}' at end of file.";
     deverror(ss.str());
  }
  else { }
  checkattribs();
  
  cout << endl;
  printwarnings();
  printerrors();


 /* cout << "name:" << name << endl;
  cout << "minDevVersion:" << minDevVersion << endl;
  cout << "targetDevVersion:" << targetDevVersion << endl;
  cout << "versionNumber:" << version_number << endl;
  cout << "debug:" << debug << endl;
  cout << "log:" << loog << endl;
  cout << "logPrecedence:" << logprecedence << endl;
  cout << "logFile:" << logfile << endl;*/
  
  if(response != 0)
     cout << "BUILD FAILED" << endl;
  else
     cout << "BUILD SUCCESSFUL" << endl;
 // cout << "scope: " << scope;
  return 0;
}