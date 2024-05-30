#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Player.h"
#include "Skydome.h"
#include "Sprite.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

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

	// x軸回転行列
	Matrix4x4 MakeRotateXMatrix(float radian);

	// y軸回転行列
	Matrix4x4 MakeRotateYMatrix(float radian);

	// z軸回転行列
	Matrix4x4 MakeRotateZMatrix(float radia);

	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	// ３次元アフィン変換行列
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 3Dモデル
	Model* model_ = nullptr;
	Model* modelBlock_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラ
	Player* player_ = nullptr;

	// 縦横ブロック配列
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//天球
	Skydome* skydome_ = nullptr;
	// 3Dモデル
	Model* modelSkydome_ = nullptr;
};