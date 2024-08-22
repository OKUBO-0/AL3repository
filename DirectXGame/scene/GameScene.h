#pragma once

#include "Audio.h"
#include "CameraController.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "MapChipField.h"
#include "Model.h"
#include "MyMath.h"
#include "Player.h" 
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"DeathParticles.h"
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
	/// ブロックの生成
	/// </summary>
	void GenerateBlokcs();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void CheckAllCollisions();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	ViewProjection viewProjection_; // ビュープロジェクション　ゲーム中に共通で一つ

	// テクスチャハンドル
	uint32_t texturHandle_ = 0;

	// Player
	Model* model_ = nullptr;   // 3Dモデル
	Player* player_ = nullptr; // 自機

	// MapBlock
	Model* blockModel_ = nullptr;

	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	// SkyDome
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// MapChipField
	MapChipField* mapChipField_;

	// CameraController
	CameraController* cameraController_ = nullptr;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// Enemy
	std::list<Enemy*> enemies_;   // 敵たち
	Model* enemyModel_ = nullptr; // 3Dモデル
	int32_t enemynumber = 3;
	// Enemy* enemy_ = nullptr;       // 敵

	//死エフェクト
	DeathParticles* deathParticles_ = nullptr;
	Model* deathParticlesModel_= nullptr;
	bool isDeathParticles = true;
};