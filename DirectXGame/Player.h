#pragma once

#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "assert.h"
#include <algorithm>
#include <numbers>
#include <algorithm>

enum class LRDirection {
	kRight,
	kLeft,
};

class Player {


public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	const Vector3& GetVelocity() const { return velocity_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;
	// モデル
	Model* model_ = nullptr;

	Vector3 velocity_ = {};
	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.2f;
	static inline const float kLimitRunSpeed = 1.0f;

	LRDirection lrDirection_ = LRDirection::kRight;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	static inline const float kTimeTurn = 0.3f;

	// 接点状態フラグ
	bool onGround_ = true;
	// 重力加速度
	static inline const float kGravityAcceleration = 0.1f;
	// 最大落下速度
	static inline const float kLimitFallSpeed = 0.5f;
	// ジャンプ初速
	static inline const float kJumpAcceleration = 1.0f;
};