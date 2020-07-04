#pragma once

#include <string>

class CommonSetting
{
public:
	enum class EInputType {
		E_INPUT_TYPE_CAMERA,		// カメラ
		E_INPUT_TYPE_FILE			// 動画ファイル
	};
	// input種別
	EInputType inputType = EInputType::E_INPUT_TYPE_CAMERA;

	// 動画ファイルパス(input種別がE_INPUT_TYPE_FILEの場合のみ有効)
	std::string inputFilePath = ".\\contents\\video\\vtest.avi";

	// カメラデバイス番号(input種別がE_INPUT_TYPE_CAMERAの場合のみ有効)
	int inputCameraDeviceNo = 0;
};

