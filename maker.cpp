#include <iostream>
#include <fstream>
#include <string.h>

#include "utils.h"

using namespace std;

int main(int argc, char** argv) {
	setlocale(LC_ALL, "");

	string exe_path(argv[1]);
	string patcher_path(argv[2]);
	long patcher_size = get_file_size(patcher_path);
	wcout << L"Размер патчера: " << patcher_size << L" байт\n";
	
	char patcher_buf[patcher_size];
	std::ifstream patcher(patcher_path, std::ios::binary);
	patcher.read(patcher_buf, patcher_size);
	patcher.close();

	std::ofstream exe(exe_path, std::ios::binary | std::ios::app);
	exe.write(patcher_buf, patcher_size);
	exe.write((char*)&patcher_size, LONG_SIZE);
	exe.write(UNCHECKED, SIG_LEN);
	exe.close();

	return 0;
}