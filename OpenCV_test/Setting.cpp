#include "Setting.h"

// �t�@�C������ݒ��ǂݍ���ŕԋp����
// ���́A�Œ�ԋp����
// .h�ŏ����l��ݒ肵�Ă���̂ŁA�㏑���������ꍇ�͂������ύX����
Setting Setting::readSettingFile()
{
	// ���ʐݒ�
	Setting setting;
	//���f�t�H���g�l��ݒ�ς݂̂��߁A����
	//setting.activeMode = EActiveMode::E_MOVE_OBJECT_DETECTER_MODE;
	setting.activeMode = EActiveMode::E_OBJECT_TRACKER_MODE;

	// ���̌��m�ݒ�
	//���f�t�H���g�l��ݒ�ς݂̂��߁A����

	// ���̒ǐՐݒ�
	setting.objectTrackSetting.enableTrackKcf = true;
	setting.objectTrackSetting.enableTrackTld = false;
	setting.objectTrackSetting.enableTrackMedianFlow = false;
	setting.objectTrackSetting.enableTrackBoosting = false;
	setting.objectTrackSetting.enableTrackMil = false;

	setting.objectTrackSetting.trackingMode = ObjectTrackSetting::ETrackingMode::E_PATARN_MATCH;	//�p�^�[���}�b�`���O
//	setting.objectTrackSetting.trackingMode = ObjectTrackSetting::ETrackingMode::E_OBJECT_TRACK;	//�g���b�L���O

	return setting;
}