#ifndef UTILS_H
#define UTILS_H

// Абсолютный путь запущенного файла. Параметр - argv[0]
std::string get_absolute_path(char* arg);
// Temp папка пользователя
std::string get_temp_dir();

#endif