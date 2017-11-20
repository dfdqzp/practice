#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>
#include <io.h>
#include <vector>

using namespace std;

int string_replace(string& buf, const string & src, const string & dest);

string file_sed(char* oldFileName, const vector<string>& srcStrVec, const string& desStr);

long long StringToHashValue(string & s);