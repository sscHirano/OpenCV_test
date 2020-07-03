#include "Setting.h"

// ƒtƒ@ƒCƒ‹‚©‚çÝ’è‚ð“Ç‚Ýž‚ñ‚Å•Ô‹p‚·‚é
// ¡‚ÍAŒÅ’è•Ô‹p‚·‚é
Setting* Setting::readSettingFile()
{
	Setting *setting = new Setting();
	setting->activeMode = EActiveMode::E_OBJECT_TRACKER_MODE;
	return setting;
}