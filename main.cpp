#include <fstream>
#include <iostream>
#include <string.h>

#include "utils.h"

const char* CHECKED = "_CHECKED_";
const char* UNCHECKED = "UNCHECKED";

#define SIG_LEN sizeof(CHECKED)
#define INT_SIZE 4
#define LONG_SIZE 8


int create_patcher(std::ifstream * self_exe, int patcher_size, std::string exe_path) {
	std::string temp_dir = get_temp_dir();
	// Записываем сам exe
	std::ofstream f_patcher(temp_dir + "patcher.exe");
	char* patcher_data = new char[patcher_size];
	self_exe.read(patcher_data, patcher_size);
	f_patcher.write(patcher_data, patcher_size);
	// Записываем путь к самой проге
	

	return 0;
}

int main(int argc, char** argv) {
	std::setlocale(0, "");
	// определяем расположение exe
	std::string abs_path = get_absolute_path(argv[0]);
	std::cout << "Abs path of app: " << abs_path;
	// читаем сигнатуру из конца файла
	std::ifstream self_exe(abs_path, std::ios::binary);
	// читаем сигнатуру
	char* signature = new char [SIG_LEN];
	self_exe.seekg(-SIG_LEN, std::ios::end);
	self_exe.read(signature, SIG_LEN);
	// читаем размер структуры (патчер или информация об окружении)
	long long struct_size;
	self_exe.seekg(-SIG_LEN - LONG_SIZE, std::ios::cur);
	self_exe.read((char*)&struct_size, LONG_SIZE);
	// ставим позицию на начало структуры
	self_exe.seekg(-LONG_SIZE - struct_size, std::ios::cur);
	
	int retval = 0;
	if (strcmp(signature, CHECKED) == 0) {
		// проверить сигнатуру с реальным положением дел
	} else if (strcmp(signature, UNCHECKED) == 0) {
		// Создание патчера и его запуск
		std::wcout << L"Создаем патчер";
		std::wcout << L"Размер патчера (в байтах) равен = " << struct_size;
		retval = create_patcher(&self_exe, struct_size, abs_path);
	} else {
		std::wcout << L"Ошибка! Запишите в оверлей данные и соответствующую сигнатуру";
		retval = 1;
	}
	
	delete[] signature;
	self_exe.close();
	
	return retval;
}