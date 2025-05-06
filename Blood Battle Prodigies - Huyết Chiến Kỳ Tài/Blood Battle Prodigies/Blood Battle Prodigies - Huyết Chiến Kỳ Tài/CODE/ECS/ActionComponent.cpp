#include "ActionComponent.h"
#include "Components.h"
#include "../game.h"
#include "../const.h"
#include"../Collision.h"
#include"SDL_mixer.h"

#define DAMAGE_EFFECT_DURATION 500 
Mix_Chunk* ActionComponent::punchSound = nullptr;
Mix_Chunk* ActionComponent::kickSound = nullptr;
Mix_Chunk* ActionComponent::runSound = nullptr;
Mix_Chunk* ActionComponent::hypnotizeSound = nullptr;
Mix_Chunk* ActionComponent::staffSound = nullptr;
Mix_Chunk* ActionComponent::kameSound = nullptr;
Mix_Chunk* ActionComponent::jumpSound = nullptr;
Mix_Chunk* ActionComponent::throwSound = nullptr;
Mix_Chunk* ActionComponent::fallSound = nullptr;
Mix_Chunk* ActionComponent::gameOverSound = nullptr;
Mix_Chunk* ActionComponent::gokuWinSound = nullptr;
Mix_Chunk* ActionComponent::chiaoWinSound = nullptr;
Mix_Chunk* ActionComponent::countdownSound = nullptr;
Entity* goku = nullptr;
Entity* chiao = nullptr;

bool gokuHypnotized = false;
Uint32 gokuHypnotizedStart = 0;
const Uint32 GOKU_HYPNOTIZED_DURATION = 5000; // thôi miên

Uint32 damageEffectTimeGoku = 0;
Uint32 damageEffectTimeChiao = 0;
Uint32 damageEffectTime = 0;

bool isFalling = false;     //XLY NGA
int fallActionIndex = -1;
int lastPlayedAction = -1; // ktra âm hd có âm thah 0

void ActionComponent::init() {
	transform = &entity->getComponent<TransformComponent>();
	sprite = &entity->getComponent<SpriteComponent>();
	currentAction = 0;
	currentFrame = 0;
	skill = 0;
	skillPhase = -1;
	lastFrameTime = SDL_GetTicks();
	isFalling = false;
	fallActionIndex = -1;
	lastPlayedAction = -1;;
	if (!punchSound) {
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			SDL_Log(" Error", Mix_GetError());
		}
		gokuWinSound = Mix_LoadWAV("assets/yeah.wav");
		chiaoWinSound = Mix_LoadWAV("assets/yeah.wav");
		gameOverSound = Mix_LoadWAV("assets/gameover.wav");
		punchSound = Mix_LoadWAV("assets/dam.wav"); //đấm
		kickSound = Mix_LoadWAV("assets/da.wav"); //đá
		hypnotizeSound = Mix_LoadWAV("assets/thoimien.wav"); // thôi miên
		staffSound = Mix_LoadWAV("assets/gay.wav"); // gậy
		kameSound = Mix_LoadWAV("assets/kame.wav"); // kame
		jumpSound = Mix_LoadWAV("assets/nhay.wav"); // nhảy
		throwSound = Mix_LoadWAV("assets/chieunem.wav"); // ném
		fallSound = Mix_LoadWAV("assets/nga.wav"); // ngã
		runSound = Mix_LoadWAV("assets/chay.wav"); // chạy
		countdownSound = Mix_LoadWAV("assets/321.wav");
	}
};
void playActionSound(int action, bool isGoku) {
	switch (action) {
	case 1:
		Mix_PlayChannel(-1, ActionComponent::runSound, 0);
		break;
	case 2:
		Mix_PlayChannel(-1, ActionComponent::jumpSound, 0);
		break;
	case 4:
	case 5:
		Mix_PlayChannel(-1, ActionComponent::punchSound, 0);
		break;
	case 6:
		Mix_PlayChannel(-1, ActionComponent::kickSound, 0);
		break;
	case 7:
		if (!isGoku) {
			Mix_PlayChannel(-1, ActionComponent::hypnotizeSound, 0);
		}
		break;
	case 8:
		if (!isGoku) {
			Mix_PlayChannel(-1, ActionComponent::throwSound, 0);
		}
		break;
	case 9:
		if (isGoku) {
			Mix_PlayChannel(-1, ActionComponent::staffSound, 0);
		}
		break;
	case 10:
	case 11:
		if (isGoku) {
			Mix_PlayChannel(-1, ActionComponent::kameSound, 0);
		}
		break;
	}
}
void ActionComponent::update() {
	int action = 0;
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	Uint32 currentTime = SDL_GetTicks();

	// -thôi miên
	if (entity->hasComponent<GokuController>() && gokuHypnotized) {
		if (currentTime - gokuHypnotizedStart < GOKU_HYPNOTIZED_DURATION) {
			return;
		}
		else {
			gokuHypnotized = false;
		}
	}
	// ngã
	if (isFalling) {
		action = fallActionIndex;
		currentAction = action;

		const int (*spriteData)[8] = entity->hasComponent<GokuController>() ? GOKU_ACTION : CHIAO_ACTION;
		int frameCount = spriteData[action][5];
		int spriteSpeed = spriteData[action][4];

		if (currentTime - lastFrameTime >= static_cast<Uint32>(spriteSpeed)) {
			if (frameCount > 0) {
				currentFrame++;
				if (currentFrame >= frameCount) {
					currentFrame = 0;
					isFalling = false;
					fallActionIndex = -1;
				}
				if (currentFrame == 0) Mix_PlayChannel(-1, fallSound, 0);
			}
			lastFrameTime = currentTime;
		}

		sprite->srcRect.x = spriteData[action][0] + currentFrame * spriteData[action][2];
		sprite->srcRect.y = spriteData[action][1];
		sprite->srcRect.w = spriteData[action][2];
		sprite->srcRect.h = spriteData[action][3];
		sprite->destRect.h = spriteData[action][3] * transform->scale;
		sprite->destRect.w = spriteData[action][2] * transform->scale;
		transform->height = spriteData[action][3];
		transform->width = spriteData[action][2];
		return;
	}

	// GOKU
	if (entity->hasComponent<GokuController>()) {
		if (skillPhase == -1) {
			if (keyState[SDL_SCANCODE_J]) {
				int energyRequired = GOKU_ACTION[4][6] * GOKU_ACTION[4][5];
				if (goku->getComponent<HPComponent>().hasEnoughEnergy(energyRequired)) {
					skillPhase = 0;
					skill = 1;
					currentFrame = 0;
					lastFrameTime = SDL_GetTicks();
				}
			}
			else if (keyState[SDL_SCANCODE_K]) {
				int energyRequired = GOKU_ACTION[6][6] * GOKU_ACTION[6][5];
				if (goku->getComponent<HPComponent>().hasEnoughEnergy(energyRequired)) {
					skillPhase = 0;
					skill = 2;
					currentFrame = 0;
					lastFrameTime = SDL_GetTicks();
				}
			}
			else if (keyState[SDL_SCANCODE_SPACE]) {
				int energyRequired = GOKU_ACTION[10][6] * GOKU_ACTION[10][5];
				if (goku->getComponent<HPComponent>().hasEnoughEnergy(energyRequired)) {
					skill = 3;
					skillPhase = 0;
					currentFrame = 0;
					lastFrameTime = SDL_GetTicks();
				}
			}
		}

		if (skillPhase != -1) {
			if (skill == 1) {
				if (skillPhase == 0)
					action = 4; // đấm 1
				else if (skillPhase == 1) {
					action = 5; // đấm 2
				}
				else action = 0;
			}
			else if (skill == 2) {
				if (skillPhase == 0)
					action = 6; // đá 1
				else if (skillPhase == 1) {
					action = 7; // đá 2
				}
				else action = 0;
			}
			else if (skill == 3) {
				if (skillPhase == 0)
					action = 10; // gd 0
				else if (skillPhase == 1) {
					action = 11; // gd 1
				}
				else if (skillPhase == 2) {
					action = 12; // gd 2
				}
				else action = 0;
			}

		}
		else {
			if (keyState[SDL_SCANCODE_A]) {

				action = 1; // chạy
				sprite->Flip = SDL_FLIP_HORIZONTAL;
			}
			else if (keyState[SDL_SCANCODE_D]) {

				action = 1; // chạy
				sprite->Flip = SDL_FLIP_NONE;
			}
			else if (!transform->scope) {
				action = 2; // nhảy
			}
			else if (keyState[SDL_SCANCODE_S]) {
				action = 3; // thủ
			}
			else if (keyState[SDL_SCANCODE_L]) {
				int energyRequired = GOKU_ACTION[9][6] * GOKU_ACTION[6][5];
				if (goku->getComponent<HPComponent>().hasEnoughEnergy(energyRequired)) {
					action = 9; // gậy
				}

			}
			else action = 0; // đứng yên
		}
	}
	// CHIAO
	else if (entity->hasComponent<ChiaotzuController>()) {
		if (keyState[SDL_SCANCODE_LEFT]) {

			action = 1; // chạy
			sprite->Flip = SDL_FLIP_HORIZONTAL;
		}
		else if (keyState[SDL_SCANCODE_RIGHT]) {

			action = 1; // chạy
			sprite->Flip = SDL_FLIP_NONE;
		}
		else if (!transform->scope) {
			action = 2; // nhảy
		}
		else if (keyState[SDL_SCANCODE_DOWN]) {
			action = 3; //thủ
		}
		else if (keyState[SDL_SCANCODE_KP_1]) {
			action = 4; // đấm
		}
		else if (keyState[SDL_SCANCODE_KP_2]) {
			action = 5; // đá
		}
		else if (keyState[SDL_SCANCODE_KP_3]) {
			int energyRequired = CHIAO_ACTION[7][6] * CHIAO_ACTION[7][5];
			if (chiao->getComponent<HPComponent>().hasEnoughEnergy(energyRequired)) {
				action = 7;
			}
		}
		else if (keyState[SDL_SCANCODE_KP_0]) {
			int energyRequired = CHIAO_ACTION[8][6] * CHIAO_ACTION[8][5];
			if (chiao->getComponent<HPComponent>().hasEnoughEnergy(energyRequired)) {
				action = 8;
			}
		}
		else action = 0;
	}

	// hd htai
	currentAction = action;
	const int (*spriteData)[8] = nullptr;
	int frameCount = 1, srcX = 0, srcY = 0, frameW = 0, frameH = 0, spriteSpeed = 0, timedelay = 0;
	double damage = 0, energy = 0;

	if (entity->hasComponent<GokuController>()) {
		spriteData = GOKU_ACTION;
		frameCount = spriteData[currentAction][5];
		srcX = spriteData[currentAction][0];
		srcY = spriteData[currentAction][1];
		frameW = spriteData[currentAction][2];
		frameH = spriteData[currentAction][3];
		spriteSpeed = spriteData[currentAction][4];
		damage = spriteData[currentAction][7];
		energy = spriteData[currentAction][6];
	}
	else {
		spriteData = CHIAO_ACTION;
		frameCount = spriteData[currentAction][5];
		srcX = spriteData[currentAction][0];
		srcY = spriteData[currentAction][1];
		frameW = spriteData[currentAction][2];
		frameH = spriteData[currentAction][3];
		spriteSpeed = spriteData[currentAction][4];
		damage = spriteData[currentAction][7];
		energy = spriteData[currentAction][6];
	}

	// Cập nhật frame animation
	if (currentTime - lastFrameTime >= static_cast<Uint32>(spriteSpeed)) {
		if (frameCount > 0) {
			currentFrame = (currentFrame + 1) % frameCount;
		}
		lastFrameTime = currentTime;

		if (skillPhase != -1 && currentFrame == 0) {
			skillPhase++;
			if (skillPhase > 3) {
				skillPhase = -1;
				currentAction = 0;
			}
		}
	}
	// ktra va chạm
	if (goku->getComponent<HPComponent>().hasEnoughEnergy(energy * frameCount)) {
		if (Collision::AABB(goku->getComponent<SpriteComponent>().destRect,
			chiao->getComponent<SpriteComponent>().destRect)) {
			if (entity->hasComponent<GokuController>()) {
				// nếu goku tấn công trúng chiao
				if (damage > 0) {
					if (!isFalling) {
						if (chiao->getComponent<ActionComponent>().currentAction != 3) {
							chiao->getComponent<HPComponent>().tryDecreaseHealth(damage);
							damageEffectTimeChiao = currentTime;
							goku->getComponent<HPComponent>().increaseEnergy(0.3);
							goku->getComponent<HPComponent>().decreaseEnergy(energy);
							chiao->getComponent<ActionComponent>().isFalling = true;
							chiao->getComponent<ActionComponent>().fallActionIndex = 6;
						}
						else {
							chiao->getComponent<HPComponent>().tryDecreaseHealth(damage * 0.3);
							goku->getComponent<HPComponent>().increaseEnergy(0.3);
							goku->getComponent<HPComponent>().decreaseEnergy(energy);

						}
						if (currentFrame == 0) {
							goku->getComponent<HPComponent>().decreaseEnergy(energy);
						}
					}
					else {
						goku->getComponent<HPComponent>().decreaseEnergy(energy);
					}
				}
			}
		}
	}


	if (chiao->getComponent<HPComponent>().hasEnoughEnergy(energy * frameCount)) {
		if (entity->hasComponent<ChiaotzuController>()) {
			if (Collision::AABB(chiao->getComponent<SpriteComponent>().destRect,
				goku->getComponent<SpriteComponent>().destRect)) {
				// thôi miên
				if (currentAction == 8) {
					gokuHypnotized = true;
					gokuHypnotizedStart = currentTime;
				}
				else if (damage > 0) {
					if (goku->getComponent<ActionComponent>().currentAction != 3) {
						goku->getComponent<HPComponent>().tryDecreaseHealth(damage);
						damageEffectTimeGoku = currentTime;
						chiao->getComponent<HPComponent>().increaseEnergy(0.3);
						chiao->getComponent<HPComponent>().decreaseEnergy(energy);
						goku->getComponent<ActionComponent>().isFalling = true;
						goku->getComponent<ActionComponent>().fallActionIndex = 8;
					}
					else {
						goku->getComponent<HPComponent>().tryDecreaseHealth(damage * 0.3);
						chiao->getComponent<HPComponent>().increaseEnergy(0.3);
						chiao->getComponent<HPComponent>().decreaseEnergy(energy);
					}
				}
			}
			if (currentFrame == 0) {
				chiao->getComponent<HPComponent>().decreaseEnergy(energy);
			}
		}
	}
	if (currentAction != lastPlayedAction && currentFrame == 0) {
		playActionSound(currentAction, entity->hasComponent<GokuController>());
		lastPlayedAction = currentAction;
	}
	sprite->srcRect.x = srcX + currentFrame * frameW;
	sprite->srcRect.y = srcY;
	sprite->srcRect.w = frameW;
	sprite->srcRect.h = frameH;
	sprite->destRect.h = frameH * transform->scale;
	sprite->destRect.w = frameW * transform->scale;
	transform->height = frameH;
	transform->width = frameW;
	if (transform->velocity.x < 0)
		sprite->Flip = SDL_FLIP_HORIZONTAL;
	else if (transform->velocity.x > 0)
		sprite->Flip = SDL_FLIP_NONE;
}