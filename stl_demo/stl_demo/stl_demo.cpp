#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <hash_map>
#include <string.h>

using namespace std;

struct hash_s {
	
	static const size_t bucket_size = 4;
	static const size_t min_buckets = 8;
	static int aa;
	size_t operator() (const string& s) const
	{
		aa += 10;
		size_t v = 0;
		for(int i = 0; i < s.size(); i++)
			v = 5 * s[i] + v;
		return v;
	}

	size_t operator() (const string& s1, const string& s2) const
	{
		return s1.compare(s2);
	}
};


char* SmartSplit(char** start, char* split)
{
	// string is empty
	if(*start == NULL || **start == '\0')
		return NULL;

	char* end = *start + strlen(*start) - 1;
	
	// trim tail split 
	while(*end == *split && *start != end)
		end--;

	if(*start != end)
		*(end + 1) = '\0';

	// trim prefix split 
	while(**start == *split && **start != '\0')
		(*start)++;

	char* ret = *start;

	while(**start != *split && **start != '\0')
		(*start)++;

	if(**start != '\0')
		**start = '\0', (*start)++;
	
	if(**start == '\0')
		*start = NULL;

	if(*ret == '\0')
		ret = NULL;

	return ret;
}

int main()
{
	char str0[] = "";
	char str1[] = ",,,1abc,,,,1edf,";
	char str2[] = ",,,,,,2abc,,,,2edf";
	char str3[] = ",,,,,,3abc,,,,3edf,,,,,";
	char str4[] = ",,,,,,,,,";
	char str5[] = "5abc,5edf,5hij,,,,";
	char str6[] = "6abc,6edf,6hij";
	char str7[] = "7abc";
	char str8[] = "8abc";

	char* start = NULL;
	char split = ',';
	char* word = NULL;
	vector<string> words;

	start = (char*)str0;
	while(word = SmartSplit(&start, &split)){
		words.push_back(string(word));
	}
	
	start = (char*)str1;
	while(word = SmartSplit(&start, &split)){
		words.push_back(string(word));
	}
	start = (char*)str2;
	while(word = SmartSplit(&start, &split)){
		words.push_back(string(word));
	}
	start = (char*)str3;
	while(word = SmartSplit(&start, &split)){
		words.push_back(string(word));
	}
	start = (char*)str4;
	while(word = SmartSplit(&start, &split)){
		words.push_back(string(word));
	}
	start = (char*)str5;
	while(word = SmartSplit(&start, &split)){
		words.push_back(string(word));
	}
	start = (char*)str6;
	while(word = SmartSplit(&start, &split)){
		words.push_back(string(word));
	}
	start = (char*)str7;
	while(word = SmartSplit(&start, &split)){
		words.push_back(string(word));
	}
	start = (char*)str8;
	while(word = SmartSplit(&start, &split)){
		words.push_back(string(word));
	}

	
	getchar();
	//int i = 1;
	//int j = 1;
	//while(i++)	
	//{
	//	switch(++j)
	//	{
	//	case 1: cout<< "1";
	//		continue;
	//	case 2: cout<< "2";
	//		break;
	//	case 3: cout<< "3";
	//		break;
	//	}
	//	j++;
	//}
	//hash_map<string, string, hash_s> store;
	//store[string("hanminzheng")] = "41270219840805xxxx";
	//store[string("qinqi")] = "41270219940805xxxx";
	//store[string("hanminzheng2")] = "41270219840805xxxx2";
	//store[string("qinqi2")] = "41270219940805xxxx2";
	//cout<< store.find(string("qinqi2"))->second;

	//string s[10];
	//int i = 0;
	//string str;
	//cin >> str;
	//stringstream in(str);

	//while(getline(in, s[i++], '#'));
	// using fmtflags as class member constants:
  //std::cout.setf (std::ios_base::hex , std::ios_base::basefield);
  //std::cout.setf (std::ios_base::showbase);
  //std::cout << 100 << '\n';

  //// using fmtflags as inherited class member constants:
  //std::cout.setf (std::ios::hex , std::ios::basefield);
  //std::cout.setf (std::ios::showbase);
  //std::cout << 100 << '\n';

  //// using fmtflags as object member constants:
  //std::cout.setf (std::cout.hex , std::cout.basefield);
  //std::cout.setf (std::cout.showbase);
  //std::cout << 100 << '\n';

  //// using fmtflags as a type:
  //std::ios_base::fmtflags ff;
  //ff = std::cout.flags();
  //ff &= ~std::cout.basefield;   // unset basefield bits
  //ff |= std::cout.hex;          // set hex
  //ff |= std::cout.showbase;     // set showbase
  //std::cout.flags(ff);
  //std::cout << 100 << '\n';

  // not using fmtflags, but using manipulators:
  std::cout << std::hex << std::showbase << 1000000000 << '\n';
	getchar();
}
//int main(){
//    string s, t, a;
//    //cin.ignore(256, '\n');
//    while(cin >> s){
//        for(int l = 1; l <= s.size()/2 && s != t; l++){
//            a = t = s.substr(0, l); 
//            for(int i = 1; t.size() < s.size(); i++)
//                t += to_string(stoll(a) + i);
//        }
//        cout << (s == t ? "YES " + a : "NO") << endl;
//    }
//    return 0;
//}
