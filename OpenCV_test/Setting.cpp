#include "Setting.h"

// �t�@�C������ݒ��ǂݍ���ŕԋp����
// ���́A�Œ�ԋp����
// .h�ŏ����l��ݒ肵�Ă���̂ŁA�㏑���������ꍇ�͂������ύX����
Setting Setting::readSettingFile()
{
	// ���ʐݒ�
	Setting setting;
	//���f�t�H���g�l��ݒ�ς݂̂��߁A����
	setting.activeMode = EActiveMode::E_MOVE_OBJECT_DETECTER_MODE;
	//setting.activeMode = EActiveMode::E_OBJECT_TRACKER_MODE;

	// ���̌��m�ݒ�
	//���f�t�H���g�l��ݒ�ς݂̂��߁A����

	// ���̒ǐՐݒ�
		
	return setting;
}