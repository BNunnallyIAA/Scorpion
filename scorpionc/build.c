#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
using namespace std;

int main()
{
  system("g++  -o  scorpionc *.cpp clib/*.cpp");
  system("sudo cp scorpionc /bin");
  system("scorpionc");
}
