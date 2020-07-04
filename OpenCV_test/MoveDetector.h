#pragma once

#include <iostream>
#include <opencv2/highgui.hpp> // highgui���W���[���̃w�b�_�[���C���N���[�h

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
	MoveDetector(MoveDetectSetting _setting)
	{
		setting = _setting;
	};
	void DetectActiveObjectFromVideo();

private:
	// �ݒ�l
	MoveDetectSetting setting;

private:
	// �C���v�b�g�ݒ�ɉ�����VideoCapture���擾
	cv::VideoCapture getVideoCapture();

	std::vector<cv::Rect> detectMoveObject(cv::Mat image1st, cv::Mat image2nd, cv::Mat image3rd);
	std::vector<cv::Rect> GetDiffRect(cv::Mat maskImage);
};

