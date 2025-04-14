#include "ActionComponent.h"
#include "Components.h"
#include "../game.h"

void ActionComponent::init() {
	transform = &entity->getComponent<TransformComponent>();
	sprite = &entity->getComponent<SpriteComponent>();
	currentAction = 0;
	currentFrame = 0;

	lastFrameTime = SDL_GetTicks();
	skillPhase = -1;
}
void ActionComponent::update() {
	int action = 0; // đứng yên
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	Uint32 currentTime = SDL_GetTicks();

	// GOKU
	if (entity->hasComponent<GokuController>()) {
		if (skillPhase == -1) {
			if (keyState[SDL_SCANCODE_SPACE]) {
				skillPhase = 0;    // chiêu đbiet
				currentFrame = 0;
				lastFrameTime = SDL_GetTicks();
			}
		}
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
		else if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_s: // thủ
				action = 3;
				break;
			case SDLK_j: // đấm
				action = 4;
				break;
			case SDLK_k: // đá	
				action = 5;
				break;
			case SDLK_l: // gậy
				action = 7;
				break;
			default:
				break;
			}
		}
		// chiêu đbiet
		else {
			if (skillPhase == 0)
				action = 8; // gd 0
			else if (skillPhase == 1) {
				action = 9; // gd 1
			}
			else if (skillPhase == 2) {
				action = 10; // gd 2
			}

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
		else if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_DOWN:  // thủ
				action = 3;
				break;
			case SDLK_KP_1: // đấm
				action = 4;
				break;
			case SDLK_KP_2:  // đá
				action = 5;
				break;
			case SDLK_KP_3: // chiêu 1 (ném)
				action = 7;
				break;
			case SDLK_KP_0:
				action = 8; // chiêu 2( thôi miên)
				break;
			default:
				break;
			}
		}
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
		damage = spriteData[currentAction][6];
		energy = spriteData[currentAction][7];
	}
	else {
		spriteData = CHIAO_ACTION;
		frameCount = spriteData[currentAction][5];
		srcX = spriteData[currentAction][0];
		srcY = spriteData[currentAction][1];
		frameW = spriteData[currentAction][2];
		frameH = spriteData[currentAction][3];
		spriteSpeed = spriteData[currentAction][4];
		damage = spriteData[currentAction][6];
		energy = spriteData[currentAction][7];
	}

	// Cập nhật frame animation
	if (currentTime - lastFrameTime >= static_cast<Uint32>(spriteSpeed)) {
		if (frameCount > 0) {
			currentFrame = (currentFrame + 1) % frameCount;
		}
		lastFrameTime = currentTime;
		// Nếu đang thực hiện chiêu đặc biệt và đã duyệt hết các frame của một vòng
		if (skillPhase != -1 && currentFrame == 0) {
			skillPhase++;
			if (skillPhase > 3) {
				skillPhase = -1;   // Reset lại chiêu đặc biệt
				currentAction = 0; // Reset về trạng thái đứng yên
			}
		}
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