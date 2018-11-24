#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

#include "utils.h"
#include "structure.h"

using namespace std;

int main(int argc, char** argv) {
	setlocale(0, "");

	cout << argv[0] << endl;
	ifstream f(argv[0], std::ios::binary);
	// считываем путь до exe
	char exe_path[MAX_PATH_SIZE]; // = "a.exe";
	f.seekg(-MAX_PATH_SIZE, std::ios::end);
	f.read(exe_path, MAX_PATH_SIZE);
	// читаем окружение
	f.seekg(-MAX_PATH_SIZE - STRUCT_SIZE, std::ios::end);
	ENVIRON* env = new ENVIRON();
	read_struct(env, &f);
	// закрываем файл
	f.close();

	wcout << L"Считанный путь: "; cout << exe_path << endl;
	
	ifstream exe(exe_path, std::ios::binary);
	// читаем сигнатуру
	char signature[SIG_LEN];
	exe.seekg(-SIG_LEN, std::ios::end);
	exe.read(signature, SIG_LEN);
	exe.close();

	string s_signature(signature);
	wcout << L"Считанная сигнатура: " << wstring(s_signature.begin(), s_signature.end()) << L'\n';

	int retval = 0;
	if (strcmp(signature, UNCHECKED) != 0) {
		wcout << L"ОШИБКА: ПАТЧЕР: сигнатура в exe не установлена";
		retval = 1;
	} else {
		ofstream exe(exe_path, std::ios::binary | std::ios::app);
		// записываем окружение
		write_struct(env, &exe);
		// записываем сигнатуру
		exe.write(CHECKED, SIG_LEN);
		exe.close();
	}
	
	return retval;
}