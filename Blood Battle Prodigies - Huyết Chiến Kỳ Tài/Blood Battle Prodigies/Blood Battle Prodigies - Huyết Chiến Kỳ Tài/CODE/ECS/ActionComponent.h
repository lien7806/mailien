#pragma once

#include "Components.h"
#include "SDL.h"
#include "../Const.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "ChiaotzuController.h"
#include "GokuController.h"
#include "SDL_mixer.h"

extern Entity* goku;
extern Entity* chiao;
extern bool gokuHypnotized;

class ActionComponent : public Component {
public:
    void init() override;
    void update() override;
    static Mix_Chunk* punchSound; // đấm
    static Mix_Chunk* kickSound; // đá
    static Mix_Chunk* hypnotizeSound; // thôi miên
    static Mix_Chunk* staffSound; // gậy
    static Mix_Chunk* kameSound;  // kame
    static Mix_Chunk* jumpSound;  // nhảy
    static Mix_Chunk* throwSound;  // ném
    static Mix_Chunk* fallSound; // ngã
    static Mix_Chunk* runSound; // chạy
    static Mix_Chunk* gameOverSound;
    static Mix_Chunk* gokuWinSound;
    static Mix_Chunk* chiaoWinSound;
    static Mix_Chunk* countdownSound;

    bool isFalling = false;
    int fallActionIndex = -1; 
private:
    SpriteComponent* sprite;
    TransformComponent* transform;
    int currentAction;
    int currentFrame;
    Uint32 lastFrameTime;
    int skill;
    int skillPhase;
    int lastPlayedAction;
};