#ifndef STRUCT_H
#define STRUCT_H

#include <fstream>

#include "utils.h"

struct ENVIRON {
	char launch_path[MAX_PATH_SIZE];
};

const int STRUCT_SIZE = MAX_PATH_SIZE;

void get_struct(ENVIRON* env);
void read_struct(ENVIRON* env, std::ifstream* f);
void write_struct(ENVIRON* env, std::ofstream* f);
void print_struct(ENVIRON* env);

#endif