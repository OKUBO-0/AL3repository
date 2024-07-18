#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "assert.h"
#include <algorithm>
#include <numbers>
#include "Matrix.h"

class Player;
class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	Vector3 GetWorldPosition();
	AABB GetAABB();
	void OnCollision(const Player* player);

private:
	WorldTransform worldTransform_; 
	Model* model_ = nullptr; 
	ViewProjection* viewProjection_ = nullptr;

	static inline const float kWalkSpeed = 0.01f;
	Vector3 velocity_ = {}; 
	
	static inline const float kWalkMotionAngleStart = 0.1f;
	static inline const float kWalkMotionAngleEnd = 0.5f;
	static inline const float kWalkMotionTime = 0.5f;
	static inline const float kEnemyWidth = 0.8f;
	static inline const float kEnemyHeight = 0.8f;
	float walkTimer_ = 0.0f;
};