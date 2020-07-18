#pragma once

#include <string>

#include <opencv2/tracking/tracker.hpp>

class ObjectTrackSetting
{
public:
	///////////////////////////////////
	// ���̒ǐՂ̐ݒ�
	///////////////////////////////////

	// ���̒ǐՃA���S���Y��ON/OFF
	bool enableTrackKcf = true;
	bool enableTrackTld = false;
	bool enableTrackMedianFlow = false;
	bool enableTrackBoosting = false;
	bool enableTrackMil = false;

	// ���̒ǐՕ��@
	enum class ETrackingMode {
		E_OBJECT_TRACK,
		E_PATARN_MATCH,
	};
	ETrackingMode trackingMode = ETrackingMode::E_PATARN_MATCH;


	//�^�[�Q�b�g�摜�t�@�C���p�X
	std::string targetFilePath = "C:\\work\\OpenCV\\contents\\�ǐՑΏ�.png";

	///////////////////////////////////
	// �p�^�[���}�b�`���O�̐ݒ�
	///////////////////////////////////
	// ���W�\��ON/OFF
	bool enableShowXYPosition = true;

	// �p�^�[���}�b�`���O�̎�@
	int matchMeathod = CV_TM_SQDIFF_NORMED;

	// �p�^�[���}�b�`���O��臒l
	double matchThreashold = 0.2;

	/////////////////////////////////////////////
	// ����ۑ�
	bool enableSaveVideo = true;
	std::string saveVideoPath = "C:\\work\\OpenCV\\contents\\output\\save_video.avi";
};

