#pragma once

#include <assert.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "Vector3.h"

enum class MapChipType {
		kBlank, // 空白
		kBlock, // ブロック
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

struct IndexSet {
	uint32_t xIndex;
	uint32_t yIndex;
};

struct Rect {
	float left;
	float right;
	float bottom;
	float top;
};

class MapChipField {

public:
	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPostionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVertical() const;
	uint32_t GetNumBlockHorizontal() const;

	IndexSet GetMapChipIndexSetByPosition(const Vector3& posotopn);
	Rect GetRectByIndex(uint32_t xindex, uint32_t yIndex);

private:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;
	// ブロック個数
	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 20;

	MapChipData mapChipData_;
};