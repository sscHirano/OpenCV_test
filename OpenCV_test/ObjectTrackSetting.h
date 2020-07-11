#pragma once

#include <string>

class ObjectTrackSetting
{
public:
	bool enableTrackKcf = true;
	bool enableTrackTld = true;
	bool enableTrackMedianFlow = true;
	bool enableTrackBoosting = true;
	bool enableTrackMil = true;

	// 物体追跡方法
	enum class ETrackingMode {
		E_OBJECT_TRACK,
		E_PATARN_MATCH,
	};
	ETrackingMode trackingMode = ETrackingMode::E_PATARN_MATCH;

	//ターゲット画像ファイルパス
	std::string targetFilePath = "C:\\work\\OpenCV\\contents\\追跡対象.png";
};

