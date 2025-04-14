#pragma once
#include"Components.h"
#include "SDL.h"
#include "../Const.h"     
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "ChiaotzuController.h"
#include "GokuController.h"

class ActionComponent : public Component {
public:
	void init() override;
	void update() override;
private:
	SpriteComponent* sprite;
	TransformComponent* transform;
	int currentAction;
	int currentFrame;
	Uint32 lastFrameTime;
	int skillPhase;	

};