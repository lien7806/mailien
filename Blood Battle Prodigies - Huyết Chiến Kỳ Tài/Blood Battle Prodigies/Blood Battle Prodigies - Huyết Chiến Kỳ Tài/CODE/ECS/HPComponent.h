#pragma once
#include "ECS.h"
#include "Components.h"
#include <algorithm>

class HPComponent : public Component {
public:
    double maxHealth;       
    double currentHealth;   // Máu htai
    double maxEnergy;           
    double currentEnergy;        // năng lượng htai
    bool stunned;
    Uint32 stunStartTime;
    Uint32 stunDelay;
    Uint32 lastHitTime;
    Uint32 hitDelay;

    double barWidth;        
    double barHeight;        
    double offsetY;       //kcach 

    HPComponent(int mHP, int width = 80, int height = 8, int offY = 10)
        : maxHealth(mHP), currentHealth(mHP),
        maxEnergy(mHP), 
        currentEnergy(0),
        barWidth(width), barHeight(height), offsetY(offY),
        stunned(false), stunStartTime(0), stunDelay(600), lastHitTime(0), hitDelay(600)
    { }
    void decreaseHealth(double amount){
        currentHealth -= amount;
        if (currentHealth < 0) currentHealth = 0;
    }
    void tryDecreaseHealth(int damage){
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastHitTime >= hitDelay){
            decreaseHealth(damage);
            lastHitTime = currentTime;
            stunned = true;
            stunStartTime = currentTime;
        }
    }
    void updateStun(){
        if (stunned) {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - stunStartTime >= stunDelay) {
                stunned = false;
            }
        }
    }
    void decreaseEnergy(double amount){
        currentEnergy -= amount;
        if (currentEnergy < 0) currentEnergy = 0;
    }
    void increaseEnergy(double amount){
        currentEnergy += amount;
        if (currentEnergy >= maxEnergy) currentEnergy = maxEnergy;
    }
    bool hasEnoughEnergy(double energyCost) const{
        return currentEnergy >= energyCost;
    }
    bool isDead() const { return currentHealth <= 0; }
};