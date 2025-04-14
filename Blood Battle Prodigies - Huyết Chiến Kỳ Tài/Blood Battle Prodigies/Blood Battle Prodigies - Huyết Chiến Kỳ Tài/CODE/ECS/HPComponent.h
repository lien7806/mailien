// thanh máu

#pragma once
#include "ECS.h"
#include "Components.h"
#include <algorithm>

class HPComponent : public Component {
public:
    int maxHealth;       // Máu tối đa
    int currentHealth;   // Máu hiện tại
    
    int maxEnergy;            // năng lượng tối đa
    int currentEnergy;        // năng lượng htai
    
    int barWidth;        // Chiều rộng thanh HP 
    int barHeight;       // Chiều cao thanh HP 
    int offsetY;         // Khoảng cách dọc 

    HPComponent(int mHP, int width = 80, int height = 8, int offY = 10)
        : maxHealth(mHP), currentHealth(mHP),
        maxEnergy(mHP), 
        currentEnergy(0), // nlg bdau
        barWidth(width), barHeight(height), offsetY(offY)
    {
    }

    bool isDead() const { return currentHealth <= 0; }
};