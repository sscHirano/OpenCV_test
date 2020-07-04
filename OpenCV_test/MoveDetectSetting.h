#pragma once
#include <string>
class MoveDetectSetting
{
public:
	enum class EMoveDetectInputType {
		E_MOVE_DETECT_INPUT_TYPE_CAMERA,		// カメラ
		E_MOVE_DETECT_INPUT_TYPE_FILE			// 動画ファイル
	};

public:
	// マスクサイズ
	int maskSize = 5;
	// 動きありと検知する閾値
	int moveDetectThresholdSize = 100;

	// input種別
	EMoveDetectInputType inputType = EMoveDetectInputType::E_MOVE_DETECT_INPUT_TYPE_CAMERA;

	// 動画ファイルパス
	std::string inputFilePath = ".\\contents\\video\\vtest.avi";

	// カメラ種別
	int inputCameraDeviceNo = 0;

public:
	MoveDetectSetting() {};

	//// マスクサイズを取得
	//int getMaskSize() { return maskSize; };
	//// 動きありと検知する閾値を取得
	//int getDetectThresholdSize() { return moveDetectThresholdSize; };

};
