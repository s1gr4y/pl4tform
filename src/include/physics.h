#ifndef PHYSICS_H
#define PHYSICS_H

#include "../include/player.h"
#include "../include/mesh.h"
#include "OBB.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

extern const double GRAVITY;
extern const double AIR_RESISTANCE;
extern const double GROUND_RESISTANCE;

void ComputePositionObj(Object obj);
void UpdatePlayerPos(Player *player, double dt);
void ComputePositionPlayer(Player *player, double dt);
void ApplyGroundResistance(vec3 vector, double dt);
void ApplyAirResistance(vec3 vector, double dt);
bool ComputeResolveCollisions(Player *player, Object *obj, float dt);

#endif
