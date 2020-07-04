// OpenCV_test.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>

// local include
#include "Setting.h"
#include "MoveDetector.h"

//関数宣言
void DetectActiveObjectFromVideo(MoveDetectSetting setting);

///////////////////////////////////////////
// 調整オプション
// マスク時のサイズ
const int MASK_SIZE = 5;
// 動体検知サイズ
const double OBJECT_SIZE_THREASHOLD = 100;


int main()
{
//    std::cout << "Hello World!\n";

	// ファイルから起動モードを読み込む
	Setting setting = Setting::readSettingFile();

	Setting::EActiveMode activeMode = setting.getActiveMode();
	switch (activeMode) {
	case Setting::EActiveMode::E_MOVE_OBJECT_DETECTER_MODE:
		// 動画を1フレームずつ取得して、差分をチェックして、画像に差分を描画する。
		std::cout << "動体検知モード開始\n";
		DetectActiveObjectFromVideo(setting.moveDetectSetting);
		break;
	case Setting::EActiveMode::E_OBJECT_TRACKER_MODE:
		// 物体追跡モード
		std::cout << "物体追跡モード開始\n";
		//TrackingObjectFromVideo();
		break;
	default:
		std::cout << "モード異常 " << (int)activeMode << "\n";
		break;
	}

	// 終了待ち
	std::cout << "終了\n";
	getchar();

	// 後処理
	return 0;
}

/*
	動体検知実行
 */
void DetectActiveObjectFromVideo(MoveDetectSetting setting)
{
	MoveDetector moveDetector(setting);
	moveDetector.DetectActiveObjectFromVideo();
	return;
}

