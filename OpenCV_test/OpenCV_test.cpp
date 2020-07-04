// OpenCV_test.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>

// local include
#include "Setting.h"
#include "MoveDetector.h"
#include "ObjectTracker.h"

//関数宣言
void detectActiveObject(CommonSetting commonSetting, MoveDetectSetting moveDetectSetting);
void trackObject(CommonSetting commonSetting, ObjectTrackSetting objectTrackSetting);

///////////////////////////////////////////

int main()
{
	// ファイルから起動モードを読み込む
	Setting setting = Setting::readSettingFile();

	Setting::EActiveMode activeMode = setting.getActiveMode();
	switch (activeMode) {
	case Setting::EActiveMode::E_MOVE_OBJECT_DETECTER_MODE:
		// 動画を1フレームずつ取得して、差分をチェックして、画像に差分を描画する。
		std::cout << "動体検知モード開始\n";
		detectActiveObject(setting.commonSetting, setting.moveDetectSetting);
		break;
	case Setting::EActiveMode::E_OBJECT_TRACKER_MODE:
		// 物体追跡モード
		std::cout << "物体追跡モード開始\n";
		trackObject(setting.commonSetting, setting.objectTrackSetting);
		break;
	default:
		std::cout << "モード異常 " << (int)activeMode << "\n";
		break;
	}

	// 終了待ち
	std::cout << "終了(Press Enter.)\n";
	getchar();

	// 後処理
	return 0;
}

/*
	動体検知実行
 */
void detectActiveObject(CommonSetting commonSetting, MoveDetectSetting moveDetectSetting)
{
	MoveDetector moveDetector(commonSetting, moveDetectSetting);
	moveDetector.startDetectingActiveObject();
}

/*
	物体追跡実行
*/
void trackObject(CommonSetting commonSetting, ObjectTrackSetting objectTrackSetting)
{
	ObjectTracker objectTracker(commonSetting, objectTrackSetting);
	objectTracker.startTracking();
}
