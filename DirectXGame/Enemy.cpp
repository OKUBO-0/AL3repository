#include "Enemy.h"

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> *3.0f / 2.0f;
	viewProjection_ = viewProjection;
	velocity_ = { -kWalkSpeed, 0, 0 };
	walkTimer_ = 0.0f;
}

void Enemy::Update() {

	walkTimer_ += 1.0f / 60.0f;

	float param = std::sinf(std::numbers::pi_v<float> *2.0f * walkTimer_ / kWalkMotionTime);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = fLerp(kWalkMotionAngleStart, kWalkMotionAngleEnd, radian);
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Enemy::Draw() {
	model_->Draw(worldTransform_, *viewProjection_); 
}