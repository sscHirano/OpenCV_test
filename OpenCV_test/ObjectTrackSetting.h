#pragma once

#include <string>

#include <opencv2/tracking/tracker.hpp>

class ObjectTrackSetting
{
public:
	///////////////////////////////////
	// 物体追跡の設定
	///////////////////////////////////

	// 物体追跡アルゴリズムON/OFF
	bool enableTrackKcf = true;
	bool enableTrackTld = false;
	bool enableTrackMedianFlow = false;
	bool enableTrackBoosting = false;
	bool enableTrackMil = false;

	// 物体追跡方法
	enum class ETrackingMode {
		E_OBJECT_TRACK,
		E_PATARN_MATCH,
	};
	ETrackingMode trackingMode = ETrackingMode::E_PATARN_MATCH;


	//ターゲット画像ファイルパス
	std::string targetFilePath = "C:\\work\\OpenCV\\contents\\追跡対象.png";

	///////////////////////////////////
	// パターンマッチングの設定
	///////////////////////////////////
	// 座標表示ON/OFF
	bool enableShowXYPosition = true;

	// パターンマッチングの手法
	int matchMeathod = CV_TM_SQDIFF_NORMED;

	// パターンマッチングの閾値
	double matchThreashold = 0.2;

	/////////////////////////////////////////////
	// 動画保存
	bool enableSaveVideo = true;
	std::string saveVideoPath = "C:\\work\\OpenCV\\contents\\output\\save_video.avi";
};

