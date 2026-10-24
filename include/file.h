#ifndef FILE_H
#define FILE_H

#include <stdio.h>

void find_path(const char* file);
void open_path(const char* path);
char* get_path(FILE* fptr, int choice);

#endif
