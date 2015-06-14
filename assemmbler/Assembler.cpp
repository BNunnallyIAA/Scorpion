#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>
#include "Assembler.h"
using namespace std;

extern bool isempty(string str);
extern long l_size;
extern int flag;

void parse();
string funcName(string token);
bool hasString(string *array, string value);
string typeToString(int type);
int tokenType(string token);
bool isDigit(char digit);
bool isLetter(char letter);
void mapTokens(string token, int tType);

#define nullptr ((void *)0)

/* Token types */
int COMMAND = 0, STRING_LITERAL = 1,
    INTEGER_LITERAL = 2,
    HEX_LITERAL = 3,
    RAM_ADDRESS = 4,
    REGISTER = 5,
    LABEL = 6,
    NULLPTR = 7,
    ARG_EMPTY = 8, // the arg is not provided
    FUNCTION_LABEL = 9,
    RESERVED = 10,
    UNKNOWN = 11,
    SPECIAL = 12;

stringstream obj; // our obj file
lmap *labels; // out labels
long linepos = 0, mapsize = 0;
stringstream token, fName, lastFunc, member_func;
bool instring = false, processedcommand, charLiteral = false;
bool inClass = false, inModule = false;
string tokens[ 4 ], _module_ = "n/a", _class_ = "n/a";
int argsize, argoverflow, tpos = 0;
long long labelOffset = 0, moduleSize = 0, classSize = 0;;
string special = "", objFile = "", WORKING_DIR = "";
long mem_func_declared = 0;

int arg1Types[ 6 ];
int arg2Types[ 6 ];
int arg3Types[ 6 ];

string instructions[82] = { // native VM instruction code
            "halt","breakp","rhalt","chalt","add","sub","mult","div","rem","inc",
            "dec","and","or","xor","not","nand","nor","xnor",
            "loadi","loadbl","push","return","call","swp","loop",
            "end","endl","do","mov","rmov","invoke","ilt","igt","ile","ige",
            "ndo","inlt","ingt","inle","inge","neg","lock","ulock","xreg","clx","rloop",
            "wloop","endwl","same","nac","sr","sl","r_mv","cpuid","rdtsc",
            "print","rand_1","rand_2","rand_3","printf","loadc","dload","t_cast",
            "sload","loadf","rln","rload","ct_int","anum","sleep","cp",
            "string", "adr", "r_load", "strcp", "e2str", "strcpi", "swi", "array",
            "aload", "aaload", "throw"
    };

bool iswhitespace(char ch)
{
  if((ch == ' ') || (ch == '\t') || (ch == '\n')
          || (ch == 10) || (ch == 13))
      return true;
 return false;
}

void umap()
{
  delete[] labels;
}

int getbounds(string token)
{
    if(token == "halt"){
            arg1Types[0] = ARG_EMPTY;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;

            arg1Types[1] = ARG_EMPTY;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = ARG_EMPTY;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   


            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 0;
        }
        else if(token == "breakp"){
            arg1Types[0] = ARG_EMPTY;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = ARG_EMPTY;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = ARG_EMPTY;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   


            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 0;
        }
        else if(token == "rhalt"){
            arg1Types[0] = ARG_EMPTY;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = ARG_EMPTY;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = ARG_EMPTY;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   


            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 0;
        }
        else if(token == "chalt"){
            arg1Types[0] = ARG_EMPTY;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = ARG_EMPTY;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = ARG_EMPTY;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   


            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 0;
        }
        else if(token == "add"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = RAM_ADDRESS;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = LABEL;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = INTEGER_LITERAL;
            arg3Types[2] = INTEGER_LITERAL;   


            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = REGISTER;
            arg3Types[5] = REGISTER;   
            return 3;
        //    System.out.println("adding etypes for add";
        }
        else if(token == "sub"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = RAM_ADDRESS;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = LABEL;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = INTEGER_LITERAL;
            arg3Types[2] = INTEGER_LITERAL;   


            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = REGISTER;
            arg3Types[5] = REGISTER;   
            return 3;
        }
        else if(token == "mult"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = RAM_ADDRESS;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = LABEL;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = INTEGER_LITERAL;
            arg3Types[2] = INTEGER_LITERAL;   


            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = REGISTER;
            arg3Types[5] = REGISTER;   
            return 3;
        }
        else if(token == "div"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = RAM_ADDRESS;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = LABEL;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = INTEGER_LITERAL;
            arg3Types[2] = INTEGER_LITERAL;   


            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = REGISTER;
            arg3Types[5] = REGISTER;   
            return 3;
        }
        else if(token == "rem") {
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = RAM_ADDRESS;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = LABEL;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = INTEGER_LITERAL;
            arg3Types[2] = INTEGER_LITERAL;   


            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = REGISTER;
            arg3Types[5] = REGISTER;   
            return 3;
        }
        else if(token == "inc"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   


            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "dec"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "and"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = RAM_ADDRESS;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = LABEL;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = INTEGER_LITERAL;
            arg3Types[2] = INTEGER_LITERAL;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = REGISTER;
            arg3Types[5] = REGISTER;   
            return 3;
        }
        else if(token == "or"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = RAM_ADDRESS;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = LABEL;   


            arg1Types[2] = REGISTER;
            arg2Types[2] = INTEGER_LITERAL;
            arg3Types[2] = INTEGER_LITERAL;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = REGISTER;
            arg3Types[5] = REGISTER;   
            return 3;
        }
        else if(token == "xor"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = RAM_ADDRESS;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = LABEL;   


            arg1Types[2] = REGISTER;
            arg2Types[2] = INTEGER_LITERAL;
            arg3Types[2] = INTEGER_LITERAL;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = REGISTER;
            arg3Types[5] = REGISTER;   
            return 3;
        }
        else if(token == "not"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "nand"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = RAM_ADDRESS;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = LABEL;   


            arg1Types[2] = REGISTER;
            arg2Types[2] = INTEGER_LITERAL;
            arg3Types[2] = INTEGER_LITERAL;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = REGISTER;
            arg3Types[5] = REGISTER;   
            return 3;
        }
        else if(token == "nor"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = RAM_ADDRESS;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = LABEL;   


            arg1Types[2] = REGISTER;
            arg2Types[2] = INTEGER_LITERAL;
            arg3Types[2] = INTEGER_LITERAL;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = REGISTER;
            arg3Types[5] = REGISTER;   
            return 3;
        }
        else if(token == "xnor"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = RAM_ADDRESS;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = LABEL;   


            arg1Types[2] = REGISTER;
            arg2Types[2] = INTEGER_LITERAL;
            arg3Types[2] = INTEGER_LITERAL;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = REGISTER;
            arg3Types[5] = REGISTER;   
            return 3;
        }
        else if(token == "loadi") {
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = INTEGER_LITERAL;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = HEX_LITERAL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = RAM_ADDRESS;
            arg2Types[2] = NULLPTR;
            arg3Types[2] = ARG_EMPTY;   


            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "loadbl"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = INTEGER_LITERAL;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = INTEGER_LITERAL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = RESERVED;
            arg3Types[2] = ARG_EMPTY;   


            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "push"){
            arg1Types[0] = LABEL;
            arg2Types[0] = LABEL;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = LABEL;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "return"){
            arg1Types[0] = LABEL;
            arg2Types[0] = INTEGER_LITERAL;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = INTEGER_LITERAL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = INTEGER_LITERAL;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "call"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   


            arg1Types[2] = LABEL;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "swp"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = LABEL;
            arg3Types[2] = ARG_EMPTY;   


            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "loop"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = INTEGER_LITERAL;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = INTEGER_LITERAL;   

            arg1Types[2] = LABEL;
            arg2Types[2] = LABEL;
            arg3Types[2] = INTEGER_LITERAL;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 3;
        }
        else if(token == "end"){
            arg1Types[0] = ARG_EMPTY;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = ARG_EMPTY;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = ARG_EMPTY;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 0;
        }
        else if(token == "endl"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = LABEL;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "do"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = REGISTER;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "mov"){
            arg1Types[0] = REGISTER;
            arg2Types[0] = INTEGER_LITERAL;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = REGISTER;
            arg2Types[1] = HEX_LITERAL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = HEX_LITERAL;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = RESERVED;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "rmov"){
            arg1Types[0] = REGISTER;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = REGISTER;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = REGISTER;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "invoke"){
            arg1Types[0] = HEX_LITERAL;
            arg2Types[0] = INTEGER_LITERAL;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = HEX_LITERAL;
            arg2Types[1] = INTEGER_LITERAL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = HEX_LITERAL;
            arg2Types[2] = INTEGER_LITERAL;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "ilt"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = REGISTER;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "igt"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = REGISTER;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "ige"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = REGISTER;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "ile"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = REGISTER;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "ndo"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "inlt"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = REGISTER;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "ingt"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = REGISTER;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "inle"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = REGISTER;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "inge"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = REGISTER;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "neg"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "lock"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "ulock"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "xreg"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "clx"){
            arg1Types[0] = ARG_EMPTY;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = ARG_EMPTY;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = ARG_EMPTY;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 0;
        }
        else if(token == "rloop"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = RAM_ADDRESS;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = LABEL;   

            arg1Types[2] = LABEL;
            arg2Types[2] = LABEL;
            arg3Types[2] = LABEL;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 3;
        }
        else if(token == "wloop"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = LABEL;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "endwl"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = LABEL;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "same"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = RAM_ADDRESS;   

            arg1Types[1] = REGISTER;
            arg2Types[1] = REGISTER;
            arg3Types[1] = REGISTER;   

            arg1Types[2] = LABEL;
            arg2Types[2] = LABEL;
            arg3Types[2] = LABEL;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 3;
        }
        else if(token == "nac"){
            arg1Types[0] = ARG_EMPTY;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = ARG_EMPTY;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = ARG_EMPTY;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 0;
        }
        else if(token == "sr"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "sl"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "r_mv"){
            arg1Types[0] = REGISTER;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = REGISTER;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = LABEL;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "cpuid"){
            arg1Types[0] = ARG_EMPTY;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = ARG_EMPTY;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = ARG_EMPTY;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 0;
        }
        else if(token == "rdtsc"){
            arg1Types[0] = ARG_EMPTY;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = ARG_EMPTY;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = ARG_EMPTY;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 0;
        }
        else if(token == "print"){
            arg1Types[0] = STRING_LITERAL;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = STRING_LITERAL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = STRING_LITERAL;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            if(Assembler::compile_only)
                flag = 3;
            return 1;
        }
        else if(token == "rand_1"){
            arg1Types[0] = ARG_EMPTY;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = ARG_EMPTY;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = ARG_EMPTY;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 0;
        }
        else if(token == "rand_2"){
            arg1Types[0] = ARG_EMPTY;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = ARG_EMPTY;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = ARG_EMPTY;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 0;
        }
        else if(token == "rand_3"){
            arg1Types[0] = ARG_EMPTY;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = ARG_EMPTY;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = ARG_EMPTY;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 0;
        }
        else if(token == "printf"){
            arg1Types[0] = STRING_LITERAL;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = STRING_LITERAL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = STRING_LITERAL;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            if(Assembler::compile_only)
                 flag = 3;
            return 1;
        }
        else if(token == "loadc"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = INTEGER_LITERAL;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = INTEGER_LITERAL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = STRING_LITERAL;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            charLiteral = true;
            return 2;
        }
        else if(token == "dload"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = INTEGER_LITERAL;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = INTEGER_LITERAL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = NULLPTR;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "t_cast"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = INTEGER_LITERAL;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = INTEGER_LITERAL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = INTEGER_LITERAL;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "sload"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = INTEGER_LITERAL;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = HEX_LITERAL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = NULLPTR;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "loadf"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = INTEGER_LITERAL;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = INTEGER_LITERAL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = NULLPTR;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "rln"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = STRING_LITERAL;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = INTEGER_LITERAL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = RAM_ADDRESS;
            arg2Types[2] = STRING_LITERAL;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            charLiteral = true;
            return 2;
        }
        else if(token == "rload"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = LABEL;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "ct_int"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = RAM_ADDRESS;
            arg2Types[2] = RAM_ADDRESS;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "anum"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = RAM_ADDRESS;
            arg2Types[2] = RAM_ADDRESS;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "sleep"){
            arg1Types[0] = INTEGER_LITERAL;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = INTEGER_LITERAL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = INTEGER_LITERAL;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "cp"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = LABEL;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "import"){
            arg1Types[0] = STRING_LITERAL;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = STRING_LITERAL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = STRING_LITERAL;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "string"){
            arg1Types[0] = RAM_ADDRESS;
            arg2Types[0] = STRING_LITERAL;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = STRING_LITERAL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = RAM_ADDRESS;
            arg2Types[2] = STRING_LITERAL;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            if(Assembler::compile_only)
                flag = 3;
            return 2;
        }
        else if(token == "adr"){
            arg1Types[0] = LABEL;
            arg2Types[0] = LABEL;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = RAM_ADDRESS;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "r_load"){
            arg1Types[0] = LABEL;
            arg2Types[0] = LABEL;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = LABEL;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = RAM_ADDRESS;
            arg2Types[2] = RAM_ADDRESS;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "&&idx_offset:"){  
            arg1Types[0] = INTEGER_LITERAL;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = INTEGER_LITERAL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = INTEGER_LITERAL;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "&&working_dir:"){    
            arg1Types[0] = STRING_LITERAL;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = STRING_LITERAL;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = STRING_LITERAL;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "strcp"){    
            arg1Types[0] = LABEL;
            arg2Types[0] = LABEL;
            arg3Types[0] = LABEL;   

            arg1Types[1] = RAM_ADDRESS;
            arg2Types[1] = RAM_ADDRESS;
            arg3Types[1] = RAM_ADDRESS;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 3;
        }
        else if(token == "e2str"){    
            arg1Types[0] = ARG_EMPTY;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = ARG_EMPTY;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = ARG_EMPTY;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 0;
        }
        else if(token == "strcpi"){      
            arg1Types[0] = LABEL;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = RAM_ADDRESS;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = REGISTER;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 1;
        }
        else if(token == "swi"){    
            arg1Types[0] = ARG_EMPTY;
            arg2Types[0] = ARG_EMPTY;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = ARG_EMPTY;
            arg2Types[1] = ARG_EMPTY;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = ARG_EMPTY;
            arg2Types[2] = ARG_EMPTY;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 0;
        }
        else if(token == "array"){      // array values 5 int
            arg1Types[0] = LABEL;
            arg2Types[0] = REGISTER;
            arg3Types[0] = RESERVED;   

            arg1Types[1] = LABEL;
            arg2Types[1] = RAM_ADDRESS;
            arg3Types[1] = RESERVED;   

            arg1Types[2] = LABEL;
            arg2Types[2] = LABEL;
            arg3Types[2] = INTEGER_LITERAL;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 3;
        }
        else if(token == "aload"){      // aload numbers 0 value
            arg1Types[0] = LABEL;
            arg2Types[0] = REGISTER;
            arg3Types[0] = RAM_ADDRESS;   

            arg1Types[1] = LABEL;
            arg2Types[1] = RAM_ADDRESS;
            arg3Types[1] = REGISTER;   

            arg1Types[2] = LABEL;
            arg2Types[2] = LABEL;
            arg3Types[2] = LABEL;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 3;
        }
        else if(token == "aaload"){      // aaload numbers 0 value
            arg1Types[0] = LABEL;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = RAM_ADDRESS;   

            arg1Types[1] = LABEL;
            arg2Types[1] = REGISTER;
            arg3Types[1] = REGISTER;   

            arg1Types[2] = LABEL;
            arg2Types[2] = LABEL;
            arg3Types[2] = LABEL;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 3;
        }
        else if(token == "throw"){ 
            arg1Types[0] = LABEL;
            arg2Types[0] = RAM_ADDRESS;
            arg3Types[0] = ARG_EMPTY;   

            arg1Types[1] = LABEL;
            arg2Types[1] = RAM_ADDRESS;
            arg3Types[1] = ARG_EMPTY;   

            arg1Types[2] = LABEL;
            arg2Types[2] = LABEL;
            arg3Types[2] = ARG_EMPTY;   

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;   

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;   

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;   
            return 2;
        }
        else if(token == "module:"){
            arg1Types[0] = LABEL;
            arg2Types[0] = SPECIAL;
            arg3Types[0] = ARG_EMPTY;

            arg1Types[1] = RAM_ADDRESS;
            arg2Types[1] = SPECIAL;
            arg3Types[1] = ARG_EMPTY;

            arg1Types[2] = LABEL;
            arg2Types[2] = SPECIAL;
            arg3Types[2] = ARG_EMPTY;

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;
            special = "{";
            return 2;
        }
        else if(token == "class:"){
            arg1Types[0] = LABEL;
            arg2Types[0] = SPECIAL;
            arg3Types[0] = ARG_EMPTY;

            arg1Types[1] = RAM_ADDRESS;
            arg2Types[1] = SPECIAL;
            arg3Types[1] = ARG_EMPTY;

            arg1Types[2] = LABEL;
            arg2Types[2] = SPECIAL;
            arg3Types[2] = ARG_EMPTY;

            arg1Types[3] = ARG_EMPTY;
            arg2Types[3] = ARG_EMPTY;
            arg3Types[3] = ARG_EMPTY;

            arg1Types[4] = ARG_EMPTY;
            arg2Types[4] = ARG_EMPTY;
            arg3Types[4] = ARG_EMPTY;

            arg1Types[5] = ARG_EMPTY;
            arg2Types[5] = ARG_EMPTY;
            arg3Types[5] = ARG_EMPTY;
            special = "{";
            return 2;
        }
        else {
            arg1Types[0] = UNKNOWN;
            arg2Types[0] = UNKNOWN;
            arg3Types[0] = UNKNOWN;   

            arg1Types[1] = UNKNOWN;
            arg2Types[1] = UNKNOWN;
            arg3Types[1] = UNKNOWN;   

            arg1Types[2] = UNKNOWN;
            arg2Types[2] = UNKNOWN;
            arg3Types[2] = UNKNOWN;   

            arg1Types[3] = UNKNOWN;
            arg2Types[3] = UNKNOWN;
            arg3Types[3] = UNKNOWN;   

            arg1Types[4] = UNKNOWN;
            arg2Types[4] = UNKNOWN;
            arg3Types[4] = UNKNOWN;   

            arg1Types[5] = UNKNOWN;
            arg2Types[5] = UNKNOWN;
            arg3Types[5] = UNKNOWN;   
            return 0;
        }
}

void add(string token)
{
   
   if(!processedcommand){
       argsize = getbounds(token);
       argoverflow = 3 - argsize;

       tokens[ tpos++ ] = token;
       processedcommand = true;
       
      if(argsize == 0){
         processedcommand = false;
         tpos = 0;
         parse();
         return;
      }
  
 }
   else{
   
       if(tpos >= argsize){
           processedcommand = false;
           tpos = 0;
           parse();
           //add(token);
       }
       else
        tokens[ tpos++ ] = token;
   }
}

void assemble(string filen, string content)
{
   cout << "nsc:   assembling file: " << filen << endl;

   fName.str("");
   fName << filen;
   if(isempty(content)){
      cout << "nsc:   error: " << filen << ": Is empty\n";
      Assembler::compile_only = true;
      return;
   }
   
   if(flag == 0){
      flag = 1;
      labels = new (nothrow) lmap[ l_size ];
      if(labels == nullptr){
        cout << "nsc:   error: Could not allocate resources.\nA fatal error has occured: Program will now exit." << endl;
        exit(-1039439);
      }
      for(int i = 0; i < 4; i++)
        tokens[i] = "";
   }

   for(int i = 0; i < content.length(); i++){
        if(content.at(i) == '\n')
                  linepos++;

        if((content.at(i) == '\'') && ((content.at(i - 1) != '/'))){
            if(instring)
                instring = false;
            else
                instring = true;
        }

        if((content.at(i) == '/') && !instring && (content.at(i + 1) == '*')){ // multi line comment
            for(int l = i; l < content.length(); i++){
                if(content.at(l) == '\n')
                    linepos++;

                if((content.at(l) == '*') && (content.at(l + 1) == '/')) { // \n
                    i++;
                    break;
                }
                l++;
            }
          i++;
        }
        else if((content.at(i) == ';')  && !instring){ // single line comment
             for(int l = i; l < content.length(); i++){
                 if((content.at(l) == 10) || (content.at(l) == '\n')) // \n
                      break;
                 l++;
             }
             linepos++;
             i++;
        }
        else if((iswhitespace(content.at(i)) || (content.at(i) == ',') || (content.at(i) == '[') || (content.at(i) == ']')) && !instring) { // if <whitespace> -> add new token
            if(token.str() != "") {
                  add(token.str()); // parse data
                 // cout << "adding token: " << t << " pos: " << tpos << endl << std::flush;
                  token.str("");
              }
         }
         else
            token << content.at(i);
   }

   
   if(inClass || inModule){
     cout << "nsc: " << fName.str() << ":" << linepos << ": error:  expected '}' at end of input." << endl;
   }
   linepos = 0;
   token.str("");
   processedcommand = false;
   tpos = 0;
   inClass = false;
   inModule = false;
   instring = false;
   cout << "obj: \n" << objFile << endl;
}

string toBinaryString(long dec)
{
    long number;
     string bin;
     char holder = ' ';
     number = dec;
     while(number != 0)
     {
	holder = number % 2 + '0';
        bin = holder + bin;
	number /= 2;
     }

     return bin;
}

long cmdIndex(string cmd) {
        if(cmd == "halt")
            return 0;
        else if(cmd == "breakp")
            return 1;
        else if(cmd == "rhalt")
            return 2;
        else if(cmd == "chalt")
            return 3;
        else if(cmd == "add")
            return 4;
        else if(cmd == "sub")
            return 5;
        else if(cmd == "mult")
            return 6;
        else if(cmd == "div")
            return 7;
        else if(cmd == "rem")
            return 8;
        else if(cmd == "inc")
            return 9;
        else if(cmd == "dec")
            return 10;
        else if(cmd == "and")
            return 11;
        else if(cmd == "or")
            return 12;
        else if(cmd == "xor")
            return 13;
        else if(cmd == "not")
            return 14;
        else if(cmd == "nand")
            return 15;
        else if(cmd == "nor")
            return 16;
        else if(cmd == "xnor")
            return 17;
        else if(cmd == "loadi")
            return 18;
        else if(cmd == "loadbl")
            return 19;
        else if(cmd == "push")
            return 24;
        else if(cmd == "return")
            return 25;
        else if(cmd == "call")
            return 26;
        else if(cmd == "swp")
            return 27;
        else if(cmd == "loop")
            return 29;
        else if(cmd == "end")
            return 30;
        else if(cmd == "endl")
            return 31;
        else if(cmd == "do")
            return 32;
        else if(cmd == "mov")
            return 33;
        else if(cmd == "rmov")
            return 34;
        else if(cmd == "invoke")
            return 35;
        else if(cmd == "ilt")
            return 36;
        else if(cmd == "igt")
            return 37;
        else if(cmd == "ile")
            return 38;
        else if(cmd == "ige")
            return 39;
        else if(cmd == "ndo")
            return 40;
        else if(cmd == "inlt")
            return 41;
        else if(cmd == "ingt")
            return 42;
        else if(cmd == "inle")
            return 43;
        else if(cmd == "inge")
            return 44;
        else if(cmd == "neg")
            return 45;
        else if(cmd == "lock")
            return 52;
        else if(cmd == "ulock")
            return 54;
        else if(cmd == "xreg")
            return 55;
        else if(cmd == "clx")
            return 56;
        else if(cmd == "rloop")
            return 57;
        else if(cmd == "wloop")
            return 58;
        else if(cmd == "endwl")
            return 59;
        else if(cmd == "same")
            return 61;
        else if(cmd == "nac")
            return 62;
        else if(cmd == "sr")
            return 63;
        else if(cmd == "sl")
            return 64;
        else if(cmd == "r_mv")
            return 65;
        else if(cmd == "cpuid")
            return 66;
        else if(cmd == "rdtsc")
            return 67;
        else if(cmd == "print")
            return 69;
        else if(cmd == "rand_1")
            return 70;
        else if(cmd == "rand_2")
            return 71;
        else if(cmd == "rand_3")
            return 72;
        else if(cmd == "printf")
            return 77;
        else if(cmd == "loadc")
            return 78;
        else if(cmd == "dload")
            return 79;
        else if(cmd == "t_cast")
            return 80;
        else if(cmd == "sload")
            return 81;
        else if(cmd == "loadf")
            return 82;
        else if(cmd == "rln")
            return 84;
        else if(cmd == "rload")
            return 85;
        else if(cmd == "ct_int")
            return 90;
        else if(cmd == "anum")
            return 92;
        else if(cmd == "sleep")
            return 93;
        else if(cmd == "cp")
            return 94;
        else if(cmd == "string")
            return 96;
        else if(cmd == "adr")
            return 97;
        else if(cmd == "r_load")
            return 98;
        else if(cmd == "strcp")
            return 99;
        else if(cmd == "e2str")
            return 100;
        else if(cmd == "strcpi")
            return 101;
        else if(cmd == "swi")
            return 102;
        else if(cmd == "array")
            return 103;
        else if(cmd == "aload")
            return 104;
        else if(cmd == "aaload")
            return 105;
        else if(cmd == "throw")
            return 106;
        else
          return -1;
}


long labelNdx = 0; // the current numeric index
long labelpos = 0;
int tier(string label)
{
    bool t1  = false, t2 = false, t3 = false;

    if(!isLetter(label.at(0)))
        return false;
   {
        bool hasLetter = false;
            for (int i = 0; i < label.size(); i++) {
                if (!isDigit(label.at(i)) && !(label.at(i) == '_') && !isLetter(label.at(i)) && !(label.at(i) == '.'))
                    t1 = false;
                if(isLetter(label.at(i)) && !hasLetter)
                    hasLetter = true;

            }

           if(!hasLetter)
               t1 = false;
           else
               t1 = true;
   }
   {
      bool hasLetter = false, reachedDot = false;
            for (int i = 0; i < label.size(); i++) {
                if( ( (label.at(i) == '.') && (i == 0) ) || ( (label.at(i) == '.') && !((i + 1) < label.size()) ) )
                    return false;

                if (!isDigit(label.at(i)) && !(label.at(i) == '_') && !isLetter(label.at(i)) && !(label.at(i) == '.'))
                    t2 = false;
                if(isLetter(label.at(i)) && !hasLetter)
                    hasLetter = true;
                if(label.at(i) == '.' && !reachedDot)
                    reachedDot = true;
                else if(label.at(i) == '.' && reachedDot)
                    reachedDot = false;
            }

           if(!hasLetter || !reachedDot)
               t2 = false;
           else if(hasLetter && reachedDot)
               t2 = true;
           else
               t2 = false;
   }
   {
      bool hasLetter = false, reachedDot = false;
      int count = 2;
            for (int i = 0; i < label.size(); i++) {
                if(((label.at(i) == '.') && (i == 0)) || ((label.at(i) == '.') && !((i + 1) < label.size())))
                    return false;

                if (!isDigit(label.at(i)) && !(label.at(i) == '_') && !isLetter(label.at(i)) && !(label.at(i) == '.'))
                    t3 = false;
                if(isLetter(label.at(i)) && !hasLetter)
                    hasLetter = true;
                if(label.at(i) == '.' && !reachedDot){
                    reachedDot = true;
                    count++;
                }
                else if(label.at(i) == '.' && !(count >= 2)){
                    reachedDot = false;
                    count++;
                }
                else
                 reachedDot = false;
            }

           if(!hasLetter || !reachedDot)
               t3 = false;
           else if(hasLetter && reachedDot)
               t3 = true;
           else
               t3 = false;
   }
     if(t1)
      return 1;
     else if(t2)
      return 2;
     else
      return 3;
}

string getpart(string part, string label, int t_size)
{
   stringstream out;
   int count = 0;
   if(part == "label"){
       if(t_size == 2){
         for(int i = 0; i < label.size(); i++){
             if(count == 1)
              out << label.at(i);

             if(label.at(i) == '.')
               count++;
         }
        return out.str();
       }
       else {
        for(int i = 0; i < label.size(); i++){
             if(count == 2)
              out << label.at(i);

             if(label.at(i) == '.')
               count++;
         }
        return out.str();
       }
   }
   else if(part == "class"){
       if(t_size == 2){
          for(int i = 0; i < label.size(); i++){
             if(label.at(i) == '.')
               break;
             else
              out << label.at(i);
         }
        return out.str();
       }
       else {
          for(int i = 0; i < label.size(); i++){
             if(count == 1)
              out << label.at(i);

             if(label.at(i) == '.')
               count++;
         }
        return out.str();
       }
   }
   else if(part == "module"){
      for(int i = 0; i < label.size(); i++){
             if(label.at(i) == '.')
               break;
             else
              out << label.at(i);
         }
        return out.str();
   }
}

bool hasClass(string Class)
{
  for(int i = 0; i < mapsize; i++){
       if(labels[i]._class == Class)
        return true;
    }
   return false;
}

bool hasModule(string Module)
{
  for(int i = 0; i < mapsize; i++){
       if(labels[i].module == Module)
        return true;
    }
   return false;
}

bool hasLabel(string label)
{
  int t_size = tier(label);
  //cout << "t size: " << t_size << " label: " << label;
  if(t_size == 1){
    for(int i = 0; i < mapsize; i++){
       if((labels[i].name == label) && (labels[i]._class == _class_) && (labels[i].module == _module_))
        return true;
    }
   return false;
  }
  else if(t_size == 2){
    string LABEL = getpart("label", label, 2);
    string CLASS = getpart("class", label, 2);

    if(!hasClass(CLASS)){
       Assembler::compile_only = true;
           if(member_func.str() != "!" && mem_func_declared == 0){
                cout << fName.str() << ": In member function " << member_func.str() << "():" << endl;
                mem_func_declared++;
           }
          cout << "nsc: " << fName.str() << ":" << linepos << ": error:  class \"" << CLASS << "\" does not exist.";
    }

    for(int i = 0; i < mapsize; i++){
       if((labels[i].name == LABEL) && (labels[i]._class == CLASS) && (labels[i].module == _module_))
        return true;
    }
   return false;
  }
  else {
    string LABEL = getpart("label", label, 3);
    string CLASS = getpart("class", label, 3);
    string MODULE = getpart("module", label, 3);

    if(!hasClass(CLASS)){
       Assembler::compile_only = true;
           if(member_func.str() != "!" && mem_func_declared == 0){
                cout << fName.str() << ": In member function " << member_func.str() << "():" << endl;
                mem_func_declared++;
           }
          cout << "nsc: " << fName.str() << ":" << linepos << ": error:  class \"" << CLASS << "\" does not exist.";
    }
    if(!hasModule(MODULE)){
       Assembler::compile_only = true;
           if(member_func.str() != "!" && mem_func_declared == 0){
                cout << fName.str() << ": In member function " << member_func.str() << "():" << endl;
                mem_func_declared++;
           }
          cout << "nsc: " << fName.str() << ":" << linepos << ": error:  class \"" << MODULE << "\" does not exist.";
    } 

    for(int i = 0; i < mapsize; i++){
       if((labels[i].name == LABEL) && (labels[i]._class == CLASS) && (labels[i].module == MODULE))
        return true;
    }
   return false;
  }
}

void createLabel(string label)
{
 int t_size = tier(label);
 if(t_size != 1){
   Assembler::compile_only = true;
   if(member_func.str() != "!" && mem_func_declared == 0){
         cout << fName.str() << ": In member function " << member_func.str() << "():" << endl;
         mem_func_declared++;
   }
     cout << "nsc: " << fName.str() << ":" << linepos << ": error:  failure to access absent label";
   return;
 }

 long idx = labelpos++;
 mapsize++;
 labels[ idx ].name = label;
 labels[ idx ].index = 50 + labelpos + labelOffset;
 labels[ idx ]._class = _class_;
 labels[ idx ].module = _module_;
}

long labelIdx(string label)
{
   int t_size = tier(label);
  if(t_size == 1){
    for(int i = 0; i < mapsize; i++){
       if((labels[i].name == label) && (labels[i]._class == _class_) && (labels[i].module == _module_))
        return labels[i].index;
    }
  }
  else if(t_size == 2){
    string LABEL = getpart("label", label, 2);
    string CLASS = getpart("class", label, 2);

    for(int i = 0; i < mapsize; i++){
       if((labels[i].name == LABEL) && (labels[i]._class == CLASS) && (labels[i].module == _module_))
        return labels[i].index;;
    }
  }
  else {
    string LABEL = getpart("label", label, 3);
    string CLASS = getpart("class", label, 3);
    string MODULE = getpart("module", label, 3);

    for(int i = 0; i < mapsize; i++){
       if((labels[i].name == LABEL) && (labels[i]._class == CLASS) && (labels[i].module == MODULE))
        return labels[i].index;
    }
  }
  return 2;
}

long strLen(string str)
{
  return 0;
}

string parseString(string str)
{
  return "";
}


string lastcommand = "", strLabel = "";
int cmdCount = 0;
/*
* Used variables for mapping
* struct map {
*   string name;
*   string module;
*   string _class;
*   long index;
* } ;
*
* stringstream obj; << this stringstream object will hold our obj file source
* map *labels;
*
* long mapsize;
* string _module_, _class_;
* string moduleList[ 10000 ];
* string classList[ 10000 ];
*
* long labelOffset, moduleSize = 0, classSize = 0;;
*
*/
void mapTokens(string token, int tType) // assemble file as we compile it
{
  cout << "mapping: " << token << " type: " << typeToString(tType) << endl;
  cmdCount++;
  if(cmdCount > 20){
    cmdCount = 0;
    obj << "\n";
  }

  if(lastcommand == "class")
  {
     _class_ = token;
     lastcommand = "";
      return;
  }
  else if(lastcommand == "module")
  {
    _module_ = token;
    lastcommand = "";
    return;
  }
  else if(lastcommand == "&&idx_offset:")
  {
    long offset = strtol(token.c_str(), NULL, 2);
    labelOffset += offset;
    lastcommand = "";
    return;
  }
  else if((lastcommand == "string") && (tType == LABEL)){
        lastcommand = "";
        strLabel = token;
        return;
  }
  else if((lastcommand == "string") && (tType == STRING_LITERAL)) {
        lastcommand = "";
        long len = strLen(token);
        if(!hasLabel(strLabel)){
            createLabel(strLabel);
            obj << " " << toBinaryString(labelIdx(strLabel));
            labelOffset += len;
            obj << " " << parseString(token);
        }
        else {
           Assembler::compile_only = true;
           if(member_func.str() != "!" && mem_func_declared == 0){
                cout << fName.str() << ": In member function " << member_func.str() << "():" << endl;
                mem_func_declared++;
           }
          cout << "nsc: " << fName.str() << ":" << linepos << ": error:  string label \"" << strLabel << "\"cannot be reassigned."; 
        }
      return;
  }

  if(tType == COMMAND){
     lastcommand = token;
     if(token == "class")
     { }
     else if(token == "classclose")
     { _class_ = "n/a"; }
     else if(token == "module")
     { }
     else if(token == "moduleclose")
     { _module_ = "n/a"; }
     else if(token == "&&idx_offset:")
     { }
     else
     {
        long dec = cmdIndex(token);
        if(dec == -1){
          obj << " u? ";
          return;
        }
        obj << " " << toBinaryString(dec);
        cout << "command " << token << " idx: " << cmdIndex(token);
     }
  }
  else if(tType == ARG_EMPTY){
    obj << " " << toBinaryString(0);
    cout << "mapping {empty}\n";
  }
  else if(tType == UNKNOWN)
    obj << " u? ";
  else if(tType == NULLPTR){
    long dec = 21937856;
    obj << " " << toBinaryString(dec);
    cout << "mapping {null}\n";
  }
  else if(tType == REGISTER){
     cout << "mapping {reg}\n";
          long dec = 0;

          if(token == "eax")
              dec = 20;
          else if(token == "ebx")
              dec = 21;
          else if(token == "sdx")
              dec = 22;
          else if(token == "bp")
              dec = 23;
          else if(token == "exc")
              dec = 24;
          else if(token == "ps")
              dec = 25;
          else if(token == "lg")
              dec = 26;
          else if(token == "lsl")
              dec = 27;
          else if(token == "sfc")
              dec = 28;
          else if(token == "scx")
              dec = 29;
          else if(token == "i1")
              dec = 30;
          else if(token == "i2")
              dec = 31;
          else if(token == "tmp")
              dec = 32;
          else if(token == "ai")
              dec = 33;
          else if(token == "ipi")
              dec = 34;
          else if(token == "ip")
              dec = 35;
          else if(token == "scr")
              dec = 36;
          else if(token == "i3")
              dec = 37;
          else if(token == "i4")
              dec = 38;
          else if(token == "i5")
              dec = 39;
          else if(token == "i6")
              dec = 40;
          else if(token == "i7")
              dec = 41;
          else if(token == "i8")
              dec = 42;
          else if(token == "i9")
              dec = 43;
          else if(token == "i10")
              dec = 44;
          else if(token == "i11")
              dec = 45;
          else if(token == "i12")
              dec = 46;
          else {
              obj << " u? ";
              return;
          }
          obj << " " << toBinaryString(dec);
     }
     else if(tType == HEX_LITERAL){
          cout << "mapping {hex}\n";
          long hex = strtol(token.c_str(), NULL, 16);
          obj << " " << toBinaryString(hex);
     }
     else if(tType == RAM_ADDRESS){
          cout << "mapping {ram}\n";
            stringstream addr;
            for(int i = 1; i < token.size(); i++) {
                addr << token.at(i);
            }
            std::string::size_type sz;
 
            string address = addr.str();
            long adr = strtol(address.c_str(), NULL, 2);
            obj << " " << toBinaryString(adr);
     }
     else if(tType == RESERVED){
          cout << "mapping {reserved}\n";
            if(token == "true")
                obj << " 1";
            else if(token == "false")
                obj << " 0";
            else if(token == "int")
                obj << " " << toBinaryString(11); //
            else if(token == "float")
                obj << " " << toBinaryString(16);//16
            else if(token == "char")
                obj << " " << toBinaryString(13); //13
            else if(token == "double")
                obj << " " << toBinaryString(12); // 12
            else if(token == "short")
                obj << " " << toBinaryString(15); //15
            else if(token == "bool")
                obj << " " << toBinaryString(14); // 14
            else
                obj << " u? ";
     }
     else if(tType == INTEGER_LITERAL){
            cout << "mapping {int}\n";
            stringstream num1, num2;
            int right = -1;
            int start = 0;
            if(token.at(0) == '#')
                start++;
            for (int i = start; i < token.length(); i++) {

                if(token.at(i) != '.')
                   num1 << token.at(i);
                else {
                    right = i + 1;
                    break;
                }
            }

            if(right != -1) { // parse right side of number
                for (int i = right; i < token.length(); i++) {
                    num2 << token.at(i);
                }

                long dec1 = strtol(num1.str().c_str(), NULL, 2);
                long dec2 = strtol(num2.str().c_str(),NULL, 2);
                obj << toBinaryString(dec1) << " " << toBinaryString(dec2);
            }
            else {
                long dec = strtol(num1.str().c_str(), NULL, 2);
                obj << " " << toBinaryString(dec);
            }
     }
     else if(tType == STRING_LITERAL){
             obj << " " << parseString(token);
          cout << "mapping {str}\n";
     }
     else if(tType == LABEL){
        cout << "mapping {lab}\n";
        if(!hasLabel(token)){
            createLabel(token);
            obj << " " << toBinaryString(labelIdx(strLabel));
        }
        else
          obj << " " << toBinaryString(labelIdx(strLabel));
     }
}

bool hasint(int *arry, int val)
{
    for(int i = 0; i < sizeof(arry); i++){
        if(arry[i] == val)
          return true;
    }
   return false;
}

bool hasExpectedType(int argn, int arg)
{
    if(argn == 0)
      return hasint(arg1Types, arg);
    if(argn == 1)
      return hasint(arg2Types, arg);
    if(argn == 2)
      return hasint(arg3Types, arg);
    return false;
}

int etype(int argn, int index)
{
    if(argn == 0)
      return arg1Types[index];
    if(argn == 1)
      return arg2Types[index];
    if(argn == 2)
      return arg3Types[index];
    return UNKNOWN;
}

bool is_flabel(string label)
{
   if(label.at(0) == '.' && label.at(label.size() - 1) == ':'){
       bool hasLetter = false;
            for (int i = 1; i < label.size() - 1; i++) {
                if (!isDigit(label.at(i)) && !(label.at(i) == '_') && !isLetter(label.at(i)))
                    return false;
                if(isLetter(label.at(i)) && !hasLetter)
                    hasLetter = true;

            }

           if(!hasLetter)
               return false;
           else
               return true;
   }
}

int errline = -1;
void parse()
{

   if(errline == -1) {
     if (hasString(instructions, tokens[0])) { // is command
                    mapTokens(tokens[0], COMMAND);

                for (int i = 0; i < argsize; i++) {
                    // parse arguments
                    if (hasExpectedType(i, tokenType(tokens[i + 1]))) {
                           if(tokens[0] == "push" && i == 0){
                              member_func.str("");
                              member_func << tokens[i + 1];
                           }
                           else if(tokens[0] == "return")
                              mem_func_declared = 0;
                           if((tokenType(tokens[i + 1]) == LABEL) && inModule && !inClass)
                              cout << "nsc: " << fName.str() << ":" << linepos << ": warning: label \"" << tokens[i + 1] << "\" will not be accessable outside of class declaration." << endl;
                           mapTokens(tokens[i + 1], tokenType(tokens[i + 1]));
                    } else {
                        Assembler::compile_only = true;
                        if(member_func.str() != "!" && mem_func_declared == 0){
                           cout << fName.str() << ": In member function " << member_func.str() << "():" << endl;
                           mem_func_declared++;
                        }
                        cout << "nsc: " << fName.str() << ":" << linepos << ": error:  expected '" << typeToString(etype(i, 0)) << "' or '" <<
                                typeToString(etype(i, 1)) << "' or '" << typeToString(etype(i, 2)) <<
                                "'. Found type \"" << typeToString(tokenType(tokens[i + 1])) << "\"." << endl;
                    }

                }

                    if (flag != 3) {
                        for (int i = 0; i < argoverflow; i++)
                             mapTokens("", ARG_EMPTY);
                    } else
                       flag = 1;

            } else if(is_flabel(tokens[0])) { // .func:(this one done manually)

                    mapTokens("push", COMMAND);
                    mapTokens(funcName(tokens[0]), LABEL);
                    mapTokens(funcName(tokens[0]) + "_b", LABEL);
                    mapTokens("", ARG_EMPTY);
                    lastFunc.str("");
                    lastFunc << tokens[0];
                    member_func.str("");
                    member_func << funcName(tokens[0]);

            }
            else if (tokens[0] == "}"){
                 if(inClass){
                   inClass = false;
                   mapTokens("classclose", COMMAND);
                 }
                 else if(inModule){
                   inModule = false;
                   mapTokens("moduleclose", COMMAND);
                }
            }
            else if (tokens[0] == "&&idx_offset:"){ // idx offset manipulation
                    if(tokenType(tokens[1]) == INTEGER_LITERAL)
                        mapTokens("idx_offset:", COMMAND);
                    else {
                        Assembler::compile_only = true;
                        if (errline == -1)
                           errline = linepos;
                        if(member_func.str() != "!" && mem_func_declared == 0){
                           cout << fName.str() << ": In member function " << member_func.str() << "():" << endl;
                           mem_func_declared++;
                        }
                        cout << "nsc: " << fName.str() << ":" << linepos << ": error:  expected integer literal. Found type \""
                        << typeToString(tokenType(tokens[1])) << "\"." << endl;
                    }

            }
            else if (tokens[0] == "&&working_dir:") { // idx offset manipulation
                stringstream dir;
                for(int i = 1; i < tokens[1].length() - 1; i++)
                    dir << tokens[1].at(i);
                WORKING_DIR = dir.str();
            }
            else if (tokens[0] == "class:") {
               if(member_func.str() != "!"){
                    Assembler::compile_only = true;
                      if (errline == -1)
                         errline = linepos;
                    cout << "nsc: " << fName.str() << ":" << linepos << ": error: cannot declare a class in a function." << endl;
               }

               if(!inClass){
                  inClass = true;
                  if(tokenType(tokens[1]) == LABEL){
                    mapTokens("class", COMMAND);
                    mapTokens(tokens[1], LABEL);
                  }
                  else {
                    Assembler::compile_only = true;
                        if (errline == -1)
                           errline = linepos;
                        cout << "nsc: " << fName.str() << ":" << linepos << ": error:  class name must be a label." << endl;
                  }
               }
               else {
                  Assembler::compile_only = true;
                        if (errline == -1)
                          errline = linepos;
  
                   cout << "nsc: " << fName.str() << ":" << linepos << ": error:  cannot nest classes." << endl;
               }
               
            }
            else if (tokens[0] == "module:") {
               if(!inModule){
                  if(!inClass){
                     inModule = true;
                     mapTokens("module", COMMAND);
                     mapTokens(tokens[1], LABEL);
                  }
                  else {
                     Assembler::compile_only = true;
                        if (errline == -1)
                          errline = linepos;

                   cout << "nsc: " << fName.str() << ":" << linepos << ": error:  cannot put a module inside a class" << endl;
                  }
               }
               else {
                   Assembler::compile_only = true;
                        if (errline == -1)
                          errline = linepos;

                   cout << "nsc: " << fName.str() << ":" << linepos << ": error:  cannot nest modules " << endl;
               }
            }
            else if (tokens[0] == "ret") { // ret <function>(this one done manually)
                    mapTokens("return", COMMAND);
                    mapTokens(funcName(lastFunc.str()), LABEL);
                    mapTokens("0", INTEGER_LITERAL);
                    mapTokens("", ARG_EMPTY);
                    member_func.str("");
                    member_func << "!";
                    mem_func_declared = 0;
            }
            else if (tokens[0] == "import") { // import statement
               // skip this shit
            }
            else {
                Assembler::compile_only = true;
                if (errline == -1)
                    errline = linepos;
                cout << "nsc: " << fName.str() << ":" << linepos << ": error:  expected instruction before \"" << tokens[0] << "\"." << endl;

            }
   }
   else if (linepos > errline) // wait until a new line has been reached before assembling
        errline = -1;

  for(int i = 0; i < 4; i++)
        tokens[i] = "";
  objFile = obj.str();
}

string funcName(string token) {
    stringstream name;
    for (int i = 1; i < token.size() - 1; i++) {
        name << token.at(i);
    }
    return name.str();
}


bool hasString(string *array, string value){

            for(int i = 0; i < 81; i++){

                if(value == array[i])
                    return true;

            }

        return false;
}

string typeToString(int type)
{
        if(type == NULLPTR)
           return "nullptr";
        else {

            if(type == RESERVED)
               return "reserved";
            if(type == SPECIAL)
               return "special";
            else if(type == STRING_LITERAL)
               return "string literal";
            else if(type == RAM_ADDRESS)
               return "ram address";
            else if(type == REGISTER)
               return "register";
            else if(type == INTEGER_LITERAL)
               return "integer";
            else if(type == HEX_LITERAL)
               return "hex literal";
            else if(type == LABEL)
               return "label";
            else if(type == COMMAND)
               return "instruction";
            else if(type == ARG_EMPTY)
               return "empty argument";
            else
                return "type unknown";
        }
}

bool isReserved(string token);
bool isSpecial(string token);
bool isStringLiteral(string token);
bool isRamAddress(string token);
bool isRegister(string token);
bool isIntegerLiteral(string token);
bool isHexLiteral(string token);
bool isLabel(string token);

int tokenType(string token){ // find type
        if(token == "null")
            return NULLPTR;
        else {

            if(isReserved(token))
                return RESERVED;
            if(isSpecial(token))
                return SPECIAL;
            else if(isStringLiteral(token))
                return STRING_LITERAL;
            else if(isRamAddress(token))
                return RAM_ADDRESS;
            else if(isRegister(token))
                return REGISTER;
            else if(isIntegerLiteral(token))
                return INTEGER_LITERAL;
            else if(isHexLiteral(token))
                return HEX_LITERAL;
            else if(isLabel(token))
                return LABEL;
            else
                return UNKNOWN;
      }
}

bool isReserved(string token) {
        if(token == "true" )
            return true;
        else if (token == "false")
            return true;
        else if (token == "int")
            return true;
        else if (token == "bool")
            return true;
        else if (token == "char")
            return true;
        else if (token == "float")
            return true;
        else if (token == "double")
            return true;
        else if (token == "short")
            return true;
        return false;
}

bool isSpecial(string token)
{
  return (token == special);
}

bool isStringLiteral(string token) {
        if(charLiteral){
            if((token.size() == 3) && (token.at(0) == '\'' && token.at(token.size() - 1) == '\''))
                return true;
            else if(((token.size() == 3) && (token.at(0) == '\'' && token.at(token.size() - 1) == '\'')) && (token.at(1) == '/'))
                return true;
        }
        else {
            if (token.at(0) == '\'' && token.at(token.size() - 1) == '\'')
                return true;
        }
        return false;
}

bool isDigit(char digit)
{
  if(digit == '0')
     return true;
  else if(digit == '1')
     return true;
  else if(digit == '2')
     return true;
  else if(digit == '3')
     return true;
  else if(digit == '4')
     return true;
  else if(digit == '5')
     return true;
  else if(digit == '6')
     return true;
  else if(digit == '7')
     return true;
  else if(digit == '8')
     return true;
  else if(digit == '9')
     return true;
  return false;
}

bool isLetter(char letter)
{
   return ( ((int) letter >= 65) && ((int) letter <= 90) ) || ( ((int) letter >= 97) && ((int) letter <= 122) );
}

bool isRamAddress(string token) {
        if(token.at(0) == '@' || token.at(0) == '*'){
            for(int i = 1; i < token.size();i++){
                if(isDigit(token.at(i))) { }
                else
                    return false;
            }
            return true;
        }
        return false;
}

bool isRegister(string token) {
        if(token == "eax")
            return true;
        else if(token == "ebx")
            return true;
        else if(token == "sdx")
            return true;
        else if(token == "bp")
            return true;
        else if(token == "exc")
            return true;
        else if(token == "ps")
            return true;
        else if(token == "lg")
            return true;
        else if(token == "lsl")
            return true;
        else if(token == "sfc")
            return true;
        else if(token == "scx")
            return true;
        else if(token == "i1")
            return true;
        else if(token == "i2")
            return true;
        else if(token == "tmp")
            return true;
        else if(token == "ai")
            return true;
        else if(token == "ipi")
            return true;
        else if(token == "ip")
            return true;
        else if(token == "scr")
            return true;
        else if(token == "i3")
            return true;
        else if(token == "i5")
            return true;
        else if(token == "i6")
            return true;
        else if(token == "i7")
            return true;
        else if(token == "i8")
            return true;
        else if(token == "i9")
            return true;
        else if(token == "i10")
            return true;
        else if(token == "i11")
            return true;
        else if(token == "i12")
            return true;
        else
            return false;
}

bool isIntegerLiteral(string token) {
        int dotCount;
        if(token.at(0) == '#'){
            dotCount = 0;
            for (int i = 1; i < token.length(); i++) {
                if (isDigit(token.at(i)) || (token.at(i) == '.')) {
                    if (token.at(i) == '.') {
                         flag = 3;
                         dotCount++;
                    }

                    if (dotCount > 1)
                        return false;
                } else
                    return false;
            }
        }
        else {
            dotCount = 0;
            for (int i = 0; i < token.length(); i++) {
                if (isDigit(token.at(i)) || (token.at(i) == '.')) {
                    if (token.at(i) == '.') {
                         flag = 3;
                         dotCount++;
                    }

                    if (dotCount > 1)
                        return false;
                } else
                    return false;
            }
        }
        return true;
}

bool array(char txt, char *arry)
{
   for(int i = 0; i < 24; i++){
       if(txt == arry[i])
          return true;
   }
  return false;
}

bool isHexadecimal(string text) {

        char hexDigits[24] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F', 'x' };

        int hexDigitsCount = 0;

        for (int i = 0; i < text.size(); i++) {
                if (!array(text.at(i), hexDigits))
                    return false;
        }

        return true;
}

bool isHexLiteral(string token) {
        if(token.at(0) == '0' && token.at(1) == 'x') {
            if (isHexadecimal(token))
                return true;
        }
        return false;
}

bool isLabel(string token) {
    bool t1  = false, t2 = false, t3 = false;

    if(!isLetter(token.at(0)))
        return false;
   {
        bool hasLetter = false;
            for (int i = 0; i < token.size(); i++) {
                if (!isDigit(token.at(i)) && !(token.at(i) == '_') && !isLetter(token.at(i)) && !(token.at(i) == '.'))
                    t1 = false;
                if(isLetter(token.at(i)) && !hasLetter)
                    hasLetter = true;

            }

           if(!hasLetter)
               t1 = false;
           else
               t1 = true;
   }
   {
      bool hasLetter = false, reachedDot = false;
            for (int i = 0; i < token.size(); i++) {
                if( ( (token.at(i) == '.') && (i == 0) ) || ( (token.at(i) == '.') && !((i + 1) < token.size()) ) )
                    return false;

                if (!isDigit(token.at(i)) && !(token.at(i) == '_') && !isLetter(token.at(i)) && !(token.at(i) == '.'))
                    t2 = false;
                if(isLetter(token.at(i)) && !hasLetter)
                    hasLetter = true;
                if(token.at(i) == '.' && !reachedDot)
                    reachedDot = true;
                else if(token.at(i) == '.' && reachedDot)
                    reachedDot = false;
            }

           if(!hasLetter || !reachedDot)
               t2 = false;
           else if(hasLetter && reachedDot)
               t2 = true;
           else
               t2 = false;
   }
   {
      bool hasLetter = false, reachedDot = false;
      int count = 2;
            for (int i = 0; i < token.size(); i++) {
                if(((token.at(i) == '.') && (i == 0)) || ((token.at(i) == '.') && !((i + 1) < token.size())))
                    return false;

                if (!isDigit(token.at(i)) && !(token.at(i) == '_') && !isLetter(token.at(i)) && !(token.at(i) == '.'))
                    t3 = false;
                if(isLetter(token.at(i)) && !hasLetter)
                    hasLetter = true;
                if(token.at(i) == '.' && !reachedDot){
                    reachedDot = true;
                    count++;
                }
                else if(token.at(i) == '.' && !(count >= 2)){
                    reachedDot = false;
                    count++;
                }
                else
                 reachedDot = false;
            }

           if(!hasLetter || !reachedDot)
               t3 = false;
           else if(hasLetter && reachedDot)
               t3 = true;
           else
               t3 = false;
   }
     if(t1 == true || t2 == true || t3 == true)
        return true;
     else // ------------------------------------------------------------
        return false;
}
