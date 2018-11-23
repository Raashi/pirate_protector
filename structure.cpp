#include <cstring>
#include <iostream>
#include <Windows.h>

#include "utils.h"
#include "structure.h"

using namespace std;


void get_launch_path(ENVIRON* env) {
	char* buffer = new char[MAX_PATH_SIZE];
	GetModuleFileName(NULL, buffer, MAX_PATH_SIZE);
	strcpy(env->launch_path, buffer);
}


void get_drive_info(ENVIRON* env) {
	
}


void get_struct(ENVIRON* env) {
	// strncpy(env->launch_path, "lolkek", MAX_PATH_SIZE - 1);
	// env->launch_path[MAX_PATH_SIZE - 1] = 0;
	get_launch_path(env);
}

void read_struct(ENVIRON* env, std::ifstream* f) {
	char path[MAX_PATH_SIZE];
	f->read(path, sizeof(env->launch_path));
	strcpy(env->launch_path, path);
}

void write_struct(ENVIRON* env, std::ofstream* f) {
	f->write(env->launch_path, sizeof(env->launch_path));
}


void print_struct(ENVIRON* env) {
	cout << "Launched path: " << env->launch_path << endl;
}
