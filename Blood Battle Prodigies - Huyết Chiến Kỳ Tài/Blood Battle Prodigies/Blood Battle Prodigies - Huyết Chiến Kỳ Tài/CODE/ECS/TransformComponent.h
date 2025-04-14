#pragma once
#include "ECS.h"
#include "Components.h"
#include "../vector2D.h"


class TransformComponent : public Component
{

public:
    vector2D position;
    vector2D velocity;

    int width = 41;
    int height = 60;
    double scale = 1.0;
    bool scope = false;


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
    /*TransformComponent(float x, float y) {
        position.x = x;
        position.y = y;

    }*/

    TransformComponent(float x, float y, int h, int w, int sc) {
        position.x = x;
        position.y = y;
        height = h;
        width = w;
        scale = sc;
        scope = false;


    }

    void init() override
    {
        velocity.x = 0;
        velocity.y = 0;
    }
    void update() override {
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;

        if (position.y + height * scale >= 560) {
            position.y = 560 - height * scale;
            velocity.y = 0;
            scope = true;
        }
        /*else {
            velocity.y += 0.5;
            scope = false;
        }*/
        if (position.x < 0) {
            position.x = 0;
        }
        if (position.x + width * scale > 1200) {
            position.x = 1200 - width * scale;
        }

    }


};
