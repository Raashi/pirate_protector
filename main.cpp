#include <fstream>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <tlhelp32.h>

#include "utils.h"
#include "structure.h"

using namespace std;

string get_patcher_path() {
	return get_temp_dir() + "signer.exe";
}

int create_signer(ifstream * self_exe, long signer_size, string exe_path) {
	// Записываем сам exe
	ofstream f_signer(get_patcher_path(), std::ios::binary | std::ios::app);
	char signer_data[signer_size];
	self_exe->read(signer_data, signer_size);
	f_signer.write(signer_data, signer_size);
	// Записываем структуру
	ENVIRON* env = new ENVIRON();
	get_struct(env);
	write_struct(env, &f_signer);
	// Записываем путь к самой проге
	char char_exe_path[MAX_PATH_SIZE];
	strncpy(char_exe_path, exe_path.c_str(), sizeof(char_exe_path));
	f_signer.write(char_exe_path, sizeof(char_exe_path));
	wcout << L"ЛОГ: Записанный в патчер путь: " << char_exe_path << L'\n';
	// закрываем файл
	f_signer.close();
	return 0;
}

int main(int argc, char** argv) {
	setlocale(0, "");
	// определяем расположение exe
	string abs_path = get_absolute_path(argv[0]);
	wcout << L"ЛОГ: абсолютный путь приложения: " << wstring(abs_path.begin(), abs_path.end()) << L"\n";
	// читаем сигнатуру из конца файла
	ifstream self_exe(abs_path, std::ios::binary);
	// читаем сигнатуру
	char signature[SIG_LEN];
	self_exe.seekg(-SIG_LEN, std::ios::end);
	self_exe.read(signature, SIG_LEN);
	// ставим позицию на начало структуры
	self_exe.seekg(-SIG_LEN, std::ios::cur);

	string s_signature(signature);
	wcout << L"ЛОГ: считанная сигнатура: " << wstring(s_signature.begin(), s_signature.end()) << L'\n';
	
	int retval = 0;
	if (strcmp(signature, CHECKED) == 0) {
		ENVIRON* env_exe = new ENVIRON();
		// ставим курсор на начало структуры
		self_exe.seekg(-STRUCT_SIZE, std::ios::cur);
		// читаем структуру
		read_struct(env_exe, &self_exe);
		// print_struct(env_exe);

		ENVIRON* env_real = new ENVIRON();
		get_struct(env_real);

		// print_struct(env_exe);
		// print_struct(env_real);

		if (compare_structs(env_exe, env_real)) {
			wcout << L"...\n...\n";
			while (true) {
				wcout << L"Beep, beep, beep, программа работает...\n";	
				sleep(1);
			}
		}
		else {
			wcout << L"...\n...\nВот так да, верните программу на место\n...\n...\n";
			system("pause");
			retval = 1;
		}
	} else if (strcmp(signature, UNCHECKED) == 0) {
		// Определяем размер сигнера и ставим курсор в начало
		long signer_size;
		self_exe.seekg(-LONG_SIZE, std::ios::cur);
		self_exe.read((char*)&signer_size, LONG_SIZE);
		self_exe.seekg(-LONG_SIZE -signer_size, std::ios::cur);
		// Создание патчера и его запуск
		wcout << L"ЛОГ: создаем signer'a\n";
		wcout << L"ЛОГ: размер signer'а (в байтах) равен = " << signer_size << L'\n';
		retval = create_signer(&self_exe, signer_size, abs_path);
		wcout << L"ЛОГ: патчер создан\nЛОГ: перезапускаем программу...\n";

		ShellExecuteA(GetDesktopWindow(), "open", get_patcher_path().c_str(), NULL, NULL, SW_HIDE);
	} else {
		wcout << L"ОШИБКА: EXE: в оверлее нет сигнатуры\n";
		retval = 1;
	}
	
	self_exe.close();
	
	return retval;
}