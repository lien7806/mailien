#pragma once
#include "SDL.h"
#include"ECS/ECS.h"
#include"ECS/ColliderComponent.h"

class Collision {
public:
    static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB) {
        return !(recA.x + recA.w < recB.x ||
            recB.x + recB.w < recA.x ||
            recA.y + recA.h < recB.y ||
            recB.y + recB.h < recA.y);
    }

};