#include <string>
#include <fstream>
#include <iostream>

#include <string.h>
 #include <sys/stat.h>

#include "utils.h"
#include "structure.h"


long get_file_size(char* filename) {
    struct stat stat_buf;
    int rc = stat(filename, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

int main(int argc, char** argv) {
	std::setlocale(0, "");
	
	std::ifstream f(argv[0], std::ios::binary);
	char* exe_path = new char [MAX_PATH_SIZE];

	f.seekg(-MAX_PATH_SIZE, std::ios::end);
	f.read(exe_path, MAX_PATH_SIZE);
	f.close();
	
	std::ifstream exe(exe_path, std::ios::binary);

	// читаем сигнатуру
	char* signature = new char [SIG_LEN];
	exe.seekg(-SIG_LEN, std::ios::end);
	exe.read(signature, SIG_LEN);
	// читаем размер структуры (патчер или информация об окружении)
	long long struct_size;
	exe.seekg(-SIG_LEN - LONG_SIZE, std::ios::cur);
	exe.read((char*)&struct_size, LONG_SIZE);

	int retval = 0;
	if (strcmp(signature, UNCHECKED) != 0) {
		std::cout << "Ошибка при патче: сигнатура в файле не установлена";
		exe.close();
		retval = 1;
	} else {
		// вынимаем сам executable
		int exe_len = get_file_size(exe_path);
		char* exe_buffer = new char [exe_len];
		exe.seekg(0, std::ios::beg);
		exe.read(exe_buffer, exe_len);
		exe.close();
		// кладем обратно
		std::ofstream exe(exe_path, std::ios::binary);
		exe.write(exe_buffer, exe_len);
		// записываем структуру
		char* structure = get_struct();
		exe.write(structure, STRUCT_SIZE);
		// записываем сигнатуру
		exe.write(CHECKED, SIG_LEN);
		exe.close();
	}

	delete[] signature;
	delete[] exe_path;

	return retval;
}