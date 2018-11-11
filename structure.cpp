#include "structure.h"

const int STRUCT_SIZE = sizeof('z');


char* get_struct() {
	char* res = new char[1];
	res[0] = 'z';
	return res;
}
