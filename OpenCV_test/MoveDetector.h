#pragma once

#include <iostream>
#include <opencv2/highgui.hpp> // highguiモジュールのヘッダーをインクルード

#include "MoveDetectSetting.h"

class MoveDetector
{

	/******************
		公開定数
	*******************/

	/******************
		公開関数
	*******************/
public:
	MoveDetector(MoveDetectSetting _setting)
	{
		setting = _setting;
	};
	void DetectActiveObjectFromVideo();

private:
	// 設定値
	MoveDetectSetting setting;

private:
	// インプット設定に応じたVideoCaptureを取得
	cv::VideoCapture getVideoCapture();

	std::vector<cv::Rect> detectMoveObject(cv::Mat image1st, cv::Mat image2nd, cv::Mat image3rd);
	std::vector<cv::Rect> GetDiffRect(cv::Mat maskImage);
};

