#include "Setting.h"

#include <opencv2/tracking/tracker.hpp>

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

	// ���͌�
	//inputType = EInputType::E_INPUT_TYPE_CAMERA;
	setting.commonSetting.inputType = CommonSetting::EInputType::E_INPUT_TYPE_FILE;
	// �J�����ԍ�
	setting.commonSetting.inputCameraDeviceNo = 0;
	// ����t�@�C���p�X(input��ʂ�E_INPUT_TYPE_FILE�̏ꍇ�̂ݗL��)
	setting.commonSetting.inputFilePath = "C:\\work\\OpenCV\\contents\\PANTO CAMERA 2.mp4";

	// ���̌��m�ݒ�
	//�p�^�[���}�b�`���O
	//setting.objectTrackSetting.matchMeathod = CV_TM_SQDIFF;
	setting.objectTrackSetting.matchMeathod = CV_TM_SQDIFF_NORMED;	//�f�t�H���g
	//setting.objectTrackSetting.matchMeathod = CV_TM_CCORR;
	//setting.objectTrackSetting.matchMeathod = CV_TM_CCORR_NORMED;
	//setting.objectTrackSetting.matchMeathod = CV_TM_CCOEFF;
	//setting.objectTrackSetting.matchMeathod = CV_TM_CCOEFF_NORMED;
	//�p�^�[���}�b�`���O臒l
	setting.objectTrackSetting.matchThreashold = 0.2;

	// ���̒ǐՐݒ�
	setting.objectTrackSetting.enableTrackKcf = true;
	setting.objectTrackSetting.enableTrackTld = false;
	setting.objectTrackSetting.enableTrackMedianFlow = false;
	setting.objectTrackSetting.enableTrackBoosting = false;
	setting.objectTrackSetting.enableTrackMil = false;

	setting.objectTrackSetting.trackingMode = ObjectTrackSetting::ETrackingMode::E_PATARN_MATCH;	//�p�^�[���}�b�`���O
//	setting.objectTrackSetting.trackingMode = ObjectTrackSetting::ETrackingMode::E_OBJECT_TRACK;	//�g���b�L���O

	// ���̒ǐ� ����ۑ�
	setting.objectTrackSetting.enableSaveVideo = false;
	setting.objectTrackSetting.saveVideoPath = "C:\\work\\OpenCV\\contents\\output\\save_video.avi";

	return setting;
}