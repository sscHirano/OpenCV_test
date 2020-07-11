#pragma once

#include <string>

#include <opencv2/tracking/tracker.hpp>

class ObjectTrackSetting
{
public:
	bool enableTrackKcf = true;
	bool enableTrackTld = true;
	bool enableTrackMedianFlow = true;
	bool enableTrackBoosting = true;
	bool enableTrackMil = true;

	// ���̒ǐՕ��@
	enum class ETrackingMode {
		E_OBJECT_TRACK,
		E_PATARN_MATCH,
	};
	ETrackingMode trackingMode = ETrackingMode::E_PATARN_MATCH;

	//�^�[�Q�b�g�摜�t�@�C���p�X
	std::string targetFilePath = "C:\\work\\OpenCV\\contents\\�ǐՑΏ�.png";
	// �p�^�[���}�b�`���O�̎�@
	int matchMeathod = CV_TM_SQDIFF_NORMED;

	// �p�^�[���}�b�`���O��臒l
	double matchThreashold = 0.2;

	/////////////////////////////////////////////
	// ����ۑ�
	bool enableSaveVideo = true;
	std::string saveVideoPath = "C:\\work\\OpenCV\\contents\\output\\save_video.avi";
};

