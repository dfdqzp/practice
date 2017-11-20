#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>
#include <io.h>

using namespace std;

int string_replace(std::string& buf, const std::string & src, const std::string & dest)
{
	int pos = 0;
	int src_len = src.size();
	int dest_len = dest.size();
	int replace_count = 0;

	while(pos != string::npos)
	{
		if((pos = buf.find(src, pos)) > 0)
			buf.replace(pos, src_len, dest), replace_count++;
	}

	return replace_count;
}

int main(int argc, char* argv[])
{
	_finddata_t fd;

	char srcStr[] =		"oooooooooooooooooooooooooooooooooooo";	
	char destStr[] =	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";


    long pf = _findfirst("*.cpp",&fd);
    do
    {
		if ((fd.attrib & _A_RDONLY) == 0) // ignore read-only file
		{
			// read all from source file
			ifstream inf(fd.name);
			std::stringstream filebuf;
			filebuf << inf.rdbuf();
			std::string buf(filebuf.str());
			inf.close();

			// replace string and recover orignal file
			if (string_replace(buf, std::string(srcStr), string(destStr)))
			{
				char newName[128];
				memset(newName, 0, 128);
				sprintf(newName, "%s.bak", fd.name);
				ofstream  newInf;
				newInf.open(newName, ios::out);
				newInf << buf;
				newInf.close();

				char cmd[100];
				sprintf(cmd, "move /Y %s %s", newName, fd.name);
				system(cmd);
			}

		} // loop one file
    }while (!_findnext(pf,&fd));

    _findclose(pf);

	return 0;
}
