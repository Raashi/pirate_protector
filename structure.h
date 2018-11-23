#ifndef STRUCT_H
#define STRUCT_H

#include <fstream>
#include <string>

#include "utils.h"

struct ENVIRON {
	std::string launch_path;
	std::string drive_serial;
};

const int STRUCT_SIZE = 2 * MAX_PATH_SIZE;

void get_struct(ENVIRON* env);
void read_struct(ENVIRON* env, std::ifstream* f);
void write_struct(ENVIRON* env, std::ofstream* f);
void print_struct(ENVIRON* env);

#endif