#include "Setting.h"

// �t�@�C������ݒ��ǂݍ���ŕԋp����
// ���́A�Œ�ԋp����
Setting* Setting::readSettingFile()
{
	Setting *setting = new Setting();
	setting->activeMode = EActiveMode::E_OBJECT_TRACKER_MODE;
	return setting;
}