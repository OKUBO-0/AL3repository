﻿#include "Skydome.h"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {

	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
}

void Skydome::Update() {}

void Skydome::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}