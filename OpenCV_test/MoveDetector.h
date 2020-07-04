#pragma once

#include <iostream>
#include <opencv2/highgui.hpp> // highgui���W���[���̃w�b�_�[���C���N���[�h

#include "CommonSetting.h"
#include "MoveDetectSetting.h"

class MoveDetector
{

	/******************
		���J�萔
	*******************/

	/******************
		���J�֐�
	*******************/
public:
	MoveDetector(CommonSetting _commonSetting, MoveDetectSetting _moveDetectSetting)
	{
		commonSetting = _commonSetting;
		moveDetectSetting = _moveDetectSetting;
	};
	// ���̌��m�J�n
	void startDetectingActiveObject();

private:
	// ���ʐݒ�
	CommonSetting commonSetting;
	// ���̌��m�ݒ�
	MoveDetectSetting moveDetectSetting;

private:
	// �C���v�b�g�ݒ�ɉ�����VideoCapture���擾
	cv::VideoCapture getVideoCapture();

	std::vector<cv::Rect> detectMoveObject(cv::Mat image1st, cv::Mat image2nd, cv::Mat image3rd);
	std::vector<cv::Rect> GetDiffRect(cv::Mat maskImage);
};

