#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	assert(model);
	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
	//ワールド変換の初期化
	worldTransform_.Initialize();

	model_ = model;
	textureHandle_ = textureHandle;
};

void Player::Update() {
	worldTransform_.UpdateMatrix();
};

void Player::Draw() {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
};