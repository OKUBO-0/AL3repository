#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "DebugCamera.h"

#include <cmath>
#include <vector>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// 3Dモデル
	Model* model_ = nullptr;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// x軸回転行列
	Matrix4x4 MakeRotateXMatrix(float radian);

	// y軸回転行列
	Matrix4x4 MakeRotateYMatrix(float radian);

	// z軸回転行列
	Matrix4x4 MakeRotateZMatrix(float radia);

	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	// ３次元アフィン変換行列
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t texturehandle_ = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};