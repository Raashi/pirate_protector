#ifndef UTILS_H
#define UTILS_H

#include <string>

const char CHECKED[] = "_CHECKED_";
const char UNCHECKED[] = "UNCHECKED";

const int SIG_LEN = sizeof(UNCHECKED);
const int INT_SIZE = sizeof(int);
const int LONG_SIZE = sizeof(long);

const int MAX_PATH_SIZE = 512;

// Абсолютный путь запущенного файла. Параметр - argv[0]
std::string get_absolute_path(char* arg);
// Temp папка пользователя
std::string get_temp_dir();
// Размер файла в байтах
long get_file_size(std::string filename);

#endif