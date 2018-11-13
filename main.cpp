#include <fstream>
#include <iostream>
#include <string.h>

#include "utils.h"

using namespace std;

int create_signer(ifstream * self_exe, long signer_size, string exe_path) {
	string temp_dir = get_temp_dir();
	// Записываем сам exe
	ofstream f_signer(/*temp_dir + */"signer.exe", std::ios::binary | std::ios::app);
	char signer_data[signer_size];
	self_exe->read(signer_data, signer_size);
	f_signer.write(signer_data, signer_size);
	// Записываем путь к самой проге
	char char_exe_path[MAX_PATH_SIZE];
	strncpy(char_exe_path, exe_path.c_str(), sizeof(char_exe_path));
	f_signer.write(char_exe_path, sizeof(char_exe_path));
	wcout << L"Записанный в патчер путь: " << char_exe_path << L'\n';
	
	f_signer.close();
	return 0;
}

int main(int argc, char** argv) {
	std::setlocale(0, "");

	// определяем расположение exe
	string abs_path = get_absolute_path(argv[0]);
	wcout << L"Абсолютный путь приложения: " << wstring(abs_path.begin(), abs_path.end()) << L"\n";
	// читаем сигнатуру из конца файла
	ifstream self_exe(abs_path, std::ios::binary);
	// читаем сигнатуру
	char signature[SIG_LEN];
	self_exe.seekg(-SIG_LEN, std::ios::end);
	self_exe.read(signature, SIG_LEN);
	// читаем размер структуры (патчер или информация об окружении)
	long struct_size;
	self_exe.seekg(-SIG_LEN - LONG_SIZE, std::ios::cur);
	self_exe.read((char*)&struct_size, LONG_SIZE);
	wcout << L"Размер структуры: " << struct_size << L'\n';
	// ставим позицию на начало структуры
	self_exe.seekg(-LONG_SIZE - struct_size, std::ios::cur);

	string s_signature(signature);
	wcout << L"Считанная сигнатура: " << wstring(s_signature.begin(), s_signature.end()) << L'\n';
	
	int retval = 0;
	if (strcmp(signature, CHECKED) == 0) {
		// проверить сигнатуру с реальным положением дел
	} else if (strcmp(signature, UNCHECKED) == 0) {
		// Создание патчера и его запуск
		wcout << L"Создаем signer\n";
		wcout << L"Размер signer'а (в байтах) равен = " << struct_size << L'\n';
		retval = create_signer(&self_exe, struct_size, abs_path);
	} else {
		wcout << L"Ошибка! Запишите в оверлей данные и соответствующую сигнатуру\n";
		retval = 1;
	}
	
	self_exe.close();
	
	return retval;
}