#include "util.h"

int string_replace(std::string& buf, const string & src, const string & dest)
{
	int pos = 0;
	int src_len = src.size();
	int dest_len = dest.size();
	int replace_count = 0;

	while(pos != string::npos)
	{
		if((pos = buf.find(src, pos)) >= 0)
			buf.replace(pos, src_len, dest), replace_count++;
	}

	return replace_count;
}

string file_sed(char* oldFileName, const vector<string>& srcStrVec, const string& desStr)
{
	ifstream inf(oldFileName);
	stringstream filebuf;
	filebuf << inf.rdbuf();
	string buf(filebuf.str());
	inf.close();

	char newFileName[128];
	memset(newFileName, 0, 128);
	int replace_count = 0;
	// replace string and recover orignal file
	for(auto v = srcStrVec.begin(); v != srcStrVec.end(); v++)
	{
		replace_count += string_replace(buf, *v, desStr);
	}

	if (replace_count)
	{
		sprintf(newFileName, "%s.bak", oldFileName);
		ofstream  newInf;
		newInf.open(oldFileName, ios::out);
		newInf << buf;
		newInf.close();

		return string(oldFileName);
	}
	else
		return string(oldFileName);
}