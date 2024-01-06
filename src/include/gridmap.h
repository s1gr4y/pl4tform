#ifndef GRIDMAP_H
#define GRIDMAP_H

#include "../../Dependencies/cglm/cglm.h"
#include "mesh.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NUMBER_OF_COLS 100
#define MAX_NUMBER_OF_ROWS 100
#define MAX_NUMBER_OF_TILE MAX_NUMBER_OF_COLS*MAX_NUMBER_OF_ROWS

typedef struct {
    uint32_t numOfObjs;
    uint32_t maxnumOfObjs;
    Object* objList;
} Tile;

typedef struct {
    uint32_t rows;
    uint32_t cols;
    float size;                     // arbitrary pick saying a tile is 50x50 units
    Tile grid[MAX_NUMBER_OF_TILE];  // we aribitrarily declare size of grid as 100x100 tiles
    uint8_t offset;
    uint32_t minNumObjPerTile;
} GridMap;


void GenerateGridMap(GridMap* map, int minNumObjPerTile);
void ResizeTileInMap(GridMap* map, uint32_t x, uint32_t y, int newSize);
void FreeGridMap(GridMap* map, int minNumObjPerTile);


#endif