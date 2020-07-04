#include "Setting.h"

// ファイルから設定を読み込んで返却する
// 今は、固定返却する
// .hで初期値を設定しているので、上書きしたい場合はこちらを変更する
Setting Setting::readSettingFile()
{
	// 共通設定
	Setting setting;
	//※デフォルト値を設定済みのため、割愛
	setting.activeMode = EActiveMode::E_MOVE_OBJECT_DETECTER_MODE;
	//setting.activeMode = EActiveMode::E_OBJECT_TRACKER_MODE;

	// 動体検知設定
	//※デフォルト値を設定済みのため、割愛

	// 物体追跡設定
		
	return setting;
}