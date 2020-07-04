#pragma once

#include <iostream>
#include <opencv2/highgui.hpp> // highguiモジュールのヘッダーをインクルード

#include "CommonSetting.h"
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
	MoveDetector(CommonSetting _commonSetting, MoveDetectSetting _moveDetectSetting)
	{
		commonSetting = _commonSetting;
		moveDetectSetting = _moveDetectSetting;
	};
	// 動体検知開始
	void startDetectingActiveObject();

private:
	// 共通設定
	CommonSetting commonSetting;
	// 動体検知設定
	MoveDetectSetting moveDetectSetting;

private:
	// インプット設定に応じたVideoCaptureを取得
	cv::VideoCapture getVideoCapture();

	std::vector<cv::Rect> detectMoveObject(cv::Mat image1st, cv::Mat image2nd, cv::Mat image3rd);
	std::vector<cv::Rect> GetDiffRect(cv::Mat maskImage);
};

