// trọng lg
#pragma once
#include "ECS.h"
#include "TransformComponent.h"
class GravityComponent : public Component{
public:
	float gravity = 1.75f; // trọng lượng
	float terminalVelocity = 5.0f; // vận tốc max
	void init() override {
		if (!entity->hasComponent<TransformComponent>()){
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
	}
private:
	TransformComponent* transform;
	void initialize() {
		init();
	}
	void update() override {
		if (transform->velocity.y < terminalVelocity){
			transform->velocity.y += gravity;
		}
		transform->position.y += transform->velocity.y;
	}
	friend class ECS;
};