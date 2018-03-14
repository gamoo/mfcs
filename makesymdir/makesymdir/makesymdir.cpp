// makesymdir.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdlib.h>
#include <Windows.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int setenv(const char *name, const char *value, int overwrite)
{
	int errcode = 0;
	if (!overwrite) {
		size_t envsize = 0;
		errcode = getenv_s(&envsize, NULL, 0, name);
		if (errcode || envsize) return errcode;
	}
	return _putenv_s(name, value);
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::ifstream infile(_T("version.ver"));
	
	if (infile.is_open()) {

		std::string a, b, c;
		std::string ver;

		int index = 0;

		while (infile >> a >> b >> c) {

			if (index != 3) {
				ver += c + ".";
			}
			else {
				ver += c;
			}
			
			if (index == 3) break;

			index++;
		}

		std::cout << ver << std::endl;

		std::string dir = "C:\\symbols\\" + ver;
		CreateDirectoryA(dir.c_str(), NULL);

		std::string direxe = "copy C:\\MobigoSecure\\*.exe " + dir + "\\*.exe";
		std::string dirdll = "copy C:\\MobigoSecure\\*.dll " + dir + "\\*.dll";
		std::string dirpdb = "copy C:\\MobigoSecure\\*.pdb " + dir + "\\*.pdb";

		system(direxe.c_str());
		system(dirdll.c_str());
		system(dirpdb.c_str());
	}

	infile.close();
	
	return 0;
}
