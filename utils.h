#ifndef UTILS_H
#define UTILS_H

extern const char* CHECKED;
extern const char* UNCHECKED;

extern const int SIG_LEN;
extern const int INT_SIZE;
extern const int LONG_SIZE;

extern const int MAX_PATH_SIZE;

// Абсолютный путь запущенного файла. Параметр - argv[0]
std::string get_absolute_path(char* arg);
// Temp папка пользователя
std::string get_temp_dir();
// Размер файла в байтах
long get_file_size(char* filename);

#endif