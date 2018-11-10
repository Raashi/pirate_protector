#include <string>
#include <fstream>
#include <iostream>

std::wstring read_file(std::string filename)
{
    std::wifstream wif(filename, std::ios::binary);
    return std::wstring((std::istreambuf_iterator<wchar_t>(wif)), {});
//is there a way i can perform search operations on rdbuf()?
}

int main(int argc, char** argv) {
	std::setlocale(0, "");

	

	std::wofstream f("D:\\Projects\\pirate_protector\\a.txt");
	f << L"лол кек чебурек";
	f.close();
	
	std::wstring content = read_file(std::string(txt_path.begin(), txt_path.end()));
	std::wcout << content;
	// std::ifstream in(txt_path);
	// std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

	// std::cout << "TXT-content is: " << contents;
}