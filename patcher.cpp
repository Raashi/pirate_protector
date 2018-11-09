#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#include <string>
#include <fstream>
#include <iostream>

#include <sstream>
#include <codecvt>


#define BUFSIZE 4096


std::wstring c2w(char* arr) {
	std::string res = arr;
	return std::wstring(res.begin(), res.end());
}


std::wstring read_file(std::string filename)
{
    std::wifstream wif(filename, std::ios::binary);
    return std::wstring((std::istreambuf_iterator<wchar_t>(wif)), {});
//is there a way i can perform search operations on rdbuf()?
}


std::wstring get_absolute_path(char* arg) {
    DWORD  retval=0;
    BOOL   success; 
    char  buffer[BUFSIZE]=TEXT(""); 
    TCHAR  buf[BUFSIZE]=TEXT(""); 
    TCHAR** lppPart={NULL};

// Retrieve the full path name for a file. 
// The file does not need to exist.

    retval = GetFullPathName(arg, BUFSIZE, buffer, lppPart);
    
    if (retval == 0) {
        // Handle an error condition.
        printf ("GetFullPathName failed (%d)\n", GetLastError());
        return L"";
    }
    else {
        printf("The full path name is:  %s\n", buffer);
        if (lppPart != NULL && *lppPart != 0) {
            _tprintf(TEXT("The final component in the path name is:  %s\n"), *lppPart);
        }
    }

	return c2w(buffer);
}


std::wstring get_txt_from_path(std::wstring path) {
	if (path.substr(path.length() - 4, 4) == L".exe")
		return path.substr(0, path.length() - 4) + L".txt";
	else
		return path + L".txt";
}


int main(int argc, char** argv) {
	std::setlocale(LC_ALL, "Russian");

	std::wstring abs_path = get_absolute_path(argv[0]);
	std::wstring txt_path = get_txt_from_path(abs_path);

	std::wofstream f("D:\\Projects\\pirate_protector\\a.txt");
	f << L"лол кек чебурек";
	f.close();
	
	std::wstring content = read_file(std::string(txt_path.begin(), txt_path.end()));
	std::wcout << content;
	// std::ifstream in(txt_path);
	// std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

	// std::cout << "TXT-content is: " << contents;
}