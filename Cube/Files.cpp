// Jeremy Vercillo
// 3/1/12
// Final Project - Mario 3D
// File loader - for levels

#include <iostream>
#include "Files.h"
#include "../common/shader_utils.h"

char* read_level(const char* filename) {
	char* leveldata = file_read(filename);
	if(leveldata == NULL) printf("Level %s not found.\n", filename);
}
