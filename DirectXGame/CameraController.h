#pragma once

#include "Model.h"

class Player;

class CameraController{

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


private:

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// player
	Player* target_ = nullptr;
	void SetTarget(Player* target) { target_ = target; }
	void Reset();

	// カメラ
	Vector3 targetOffset_ = { 0, 0, -15.0f };

};