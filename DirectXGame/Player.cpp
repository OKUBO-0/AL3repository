#define NOMINMAX
#include "Player.h"
#include "MapChipField.h"
#include <DebugText.h>

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	viewProjection_ = viewProjection;
};

void Player::Update() {

	PlayerMove();

	// 衝突判定を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;
	// マップ衝突チェック
	MapCollision(collisionMapInfo);
	// 移動
	CeilingCollisionMove(collisionMapInfo);
	PlayerCollisionMove(collisionMapInfo);
	worldTransform_.UpdateMatrix();

	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;

		// 左右の角度テーブル
		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float> *3.0f / 2.0f,
		};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = destinationRotationY * EaseOutSine(turnTimer_);
		;
		;
	}
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);
};

void Player::PlayerMove() {
	if (onGround_) {
		// 移動入力
		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 左右加速
			Vector3 accceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kLimitRunSpeed;
				}

				accceleration.x += kAcceleration;

			}
			else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kLimitRunSpeed;
				}

				accceleration.x -= kAcceleration;
			}
			velocity_.x += accceleration.x;
			velocity_.y += accceleration.y;
			velocity_.z += accceleration.z;

			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		}
		else {

			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y *= (1.0f - kAttenuation);
			velocity_.z *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {

			velocity_.x += 0;
			velocity_.y += kJumpAcceleration;
			velocity_.z += 0;
		}

	}
	else {
		// 落下速度
		velocity_.x += 0;
		velocity_.y += -kGravityAcceleration;
		velocity_.z += 0;
		// 落下速度制限

		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;

		// 左右の角度テーブル
		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float> *3.0f / 2.0f,
		};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = destinationRotationY * EaseOutSine(turnTimer_);
		;
		;
	}
	// 着地フラグ
	bool landing = false;
	// 地面との当たり判定
	// 落下途中？
	if (velocity_.y < 0) {
		// 座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.0f) {

			landing = true;
		}
	}

	// 接地判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {

			onGround_ = false;
		}
	}
	else {
		//着地
		if (landing) {
			//めり込み排斤
			worldTransform_.translation_.y = 1.0f;
			//摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			//下方向速度をリセット
			velocity_.y = 0.0f;
			//接地状態に移行
			onGround_ = true;
		}

	}
}

void Player::MapCollision(CollisionMapInfo& info) {

	CollisionMapInfoTop(info);
	/*CollisionMapInfoBootm(info);
	CollisionMapInfoRight(info);
	CollisionMapInfoLeft( info);*/
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {
		{+kWidth / 2.0f, -kHeight / 2.0f, 0}, //右下
		{-kWidth / 2.0f, -kHeight / 2.0f, 0}, //左下
		{+kWidth / 2.0f, +kHeight / 2.0f, 0}, //右下
		{-kWidth / 2.0f, +kHeight / 2.0f, 0}  //左上
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::PlayerCollisionMove(const CollisionMapInfo& info) {
	// 移動
	worldTransform_.translation_.x += info.move.x;
	worldTransform_.translation_.y += info.move.y;
	worldTransform_.translation_.z += info.move.z;
}

void Player::CeilingCollisionMove(const CollisionMapInfo& info) {
	// 天井に当たった?
	if (info.ceiling) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

void Player::CollisionMapInfoTop(CollisionMapInfo& info) {
	if (info.move.y <= 0) {
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真上の当たり判定
	bool hit = false;
	// 左上点の判定
	IndexSet indexSet;
	indexSet = mapChipFiled_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipFiled_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上点の判定
	indexSet = mapChipFiled_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipFiled_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {

		// めり込みを排除する方向に移動量を設定する
		Vector3 topCenterPosition;
		topCenterPosition.x = (positionsNew[kLeftTop].x + positionsNew[kRightTop].x) / 2;
		topCenterPosition.y = positionsNew[kLeftTop].y; // 左右で高さは同じ
		topCenterPosition.z = (positionsNew[kLeftTop].z + positionsNew[kRightTop].z) / 2;

		indexSet = mapChipFiled_->GetMapChipIndexSetByPosition(topCenterPosition);
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipFiled_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
		// 天井に当たったらことを記録する
		info.ceiling = true;
	}
}

void Player::CollisionMapInfoBootm(CollisionMapInfo& info) {
	if (info.move.y >= 0) {
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真上の当たり判定
	bool hit = false;
	// 左上点の判定
	IndexSet indexSet;
	indexSet = mapChipFiled_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipFiled_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上点の判定
	indexSet = mapChipFiled_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipFiled_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {

		// めり込みを排除する方向に移動量を設定する
		Vector3 bottomCenterPosition;
		bottomCenterPosition.x = (positionsNew[kLeftBottom].x + positionsNew[kRightBottom].x) / 2;
		bottomCenterPosition.y = positionsNew[kLeftBottom].y; // 左右で高さは同じ
		bottomCenterPosition.z = (positionsNew[kLeftBottom].z + positionsNew[kRightBottom].z) / 2;

		indexSet = mapChipFiled_->GetMapChipIndexSetByPosition(bottomCenterPosition);
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipFiled_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
		// 天井に当たったらことを記録する
		info.ceiling = true;
	}
}

void Player::CollisionMapInfoRight(CollisionMapInfo& info) {
	if (info.move.x <= 0) {
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = CornerPosition(
			worldTransform_.translation_ + info.move,
			static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//当たり判定
	bool hit = false;
	//右上の角の判定
	IndexSet indexSet;
	indexSet = mapChipFiled_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipFiled_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipFiled_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipFiled_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit == true) {
		Vector3 rightCenterPosition;
		rightCenterPosition.x = positionsNew[kRightTop].x; //上下で座標は同じ
		rightCenterPosition.y = (positionsNew[kRightTop].y + positionsNew[kRightBottom].y) / 2;
		rightCenterPosition.z = (positionsNew[kRightTop].z + positionsNew[kRightBottom].z) / 2;

		indexSet = mapChipFiled_->GetMapChipIndexSetByPosition(rightCenterPosition);
		Rect rect = mapChipFiled_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - kWidth / 2 + kBlank);
		//地面に当たったことを記録する
		info.ceiling = true;
	}
}

void Player::CollisionMapInfoLeft(CollisionMapInfo& info) {
	if (info.move.x >= 0) {
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = CornerPosition(
			worldTransform_.translation_ + info.move,
			static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//当たり判定
	bool hit = false;
	//左上の角の判定
	IndexSet indexSet;
	indexSet = mapChipFiled_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipFiled_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	indexSet = mapChipFiled_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipFiled_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit == true) {
		Vector3 leftCenterPosition;
		leftCenterPosition.x = positionsNew[kLeftTop].x; //上下で座標は同じ
		leftCenterPosition.y = (positionsNew[kLeftTop].y + positionsNew[kLeftBottom].y) / 2;
		leftCenterPosition.z = (positionsNew[kLeftTop].z + positionsNew[kLeftBottom].z) / 2;

		indexSet = mapChipFiled_->GetMapChipIndexSetByPosition(leftCenterPosition);

		//めり込み先ブロックの範囲矩形
		Rect rect = mapChipFiled_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::min(0.0f, rect.right - worldTransform_.translation_.x + kWidth / 2 + kBlank);
		//地面に当たったことを記録する
		info.ceiling = true;
	}
}

float Player::EaseOutSine(float x) {
	return cosf((x * std::numbers::pi_v<float>) / 2); 
}