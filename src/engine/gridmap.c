#include "gridmap.h"

void GenerateGridMap(GridMap* map, int minNumObjPerTile) {  // should be big enough to handle around 20-30 entities
    // we will instead allocate a list of
    map->rows = MAX_NUMBER_OF_ROWS;
    map->cols = MAX_NUMBER_OF_COLS;
    map->offset = sizeof(Tile);
    map->size = 50.0f;

    map->minNumObjPerTile = minNumObjPerTile;
    for (int i = 0; i < map->rows*map->cols; i++) {
        map->grid[i].objList = (Object*)malloc(sizeof(Object) * map->minNumObjPerTile);
        map->grid[i].maxnumOfObjs = minNumObjPerTile;
    }
}

void ResizeTileInMap(GridMap* map, uint32_t x, uint32_t y, int newSize) {
    //map->minNumObjPerTile = map->minNumObjPerTile
    map->grid[(y*map->rows) + x].objList = realloc(map->grid[(y*map->rows) + x].objList, newSize);
    map->grid[(y*map->rows) + x].maxnumOfObjs = newSize;
}

void FreeGridMap(GridMap* map, int minNumObjPerTile) {
    for (int i = 0; i < map->rows*map->cols; i++) {
        free(map->grid[i].objList);
    }
}