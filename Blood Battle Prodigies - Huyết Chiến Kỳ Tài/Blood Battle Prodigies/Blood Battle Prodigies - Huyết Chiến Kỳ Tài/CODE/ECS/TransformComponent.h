#pragma once
#include "ECS.h"
#include "Components.h"
#include "../vector2D.h"


class TransformComponent : public Component
{

public:
    vector2D position;
    vector2D velocity;

    int height = 32;
    int width = 32;
    double scale = 1.0;


    int speed = 3;

    TransformComponent() {
        position.x = 0.0f;
        position.y = 0.0f;
    }

    TransformComponent(int sc) {
        position.x = 0.0f;
        position.y = 0.0f;
        scale = sc;

    }
    TransformComponent(float x, float y) {
        position.x = x;
        position.y = y;

    }

    TransformComponent(float x, float y, int h, int w, int sc) {
        position.x = x;
        position.y = y;
        height = h;
        width = w;
        scale = sc;


    }

    void init() override
    {
        velocity.x = 0;
        velocity.y = 0;
    }
    void update() override {
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
    }


};



