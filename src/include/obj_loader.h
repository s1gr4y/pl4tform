#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int lineNum;
    FILE* f_ptr;
    char buff[255];
} FileReader;

void readObjFile(char path[], int path_length, float** data, unsigned int* dataSize, int** indices, unsigned int* indexSize);

#endif