#include "Setting.h"

// ファイルから設定を読み込んで返却する
// 今は、固定返却する
Setting* Setting::readSettingFile()
{
	Setting *setting = new Setting();
	setting->activeMode = EActiveMode::E_OBJECT_TRACKER_MODE;
	return setting;
}