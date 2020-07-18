#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking/tracker.hpp>
#include <opencv2/imgproc.hpp>

#include "CommonSetting.h"
#include "ObjectTrackSetting.h"

class TargetInfo;

class ObjectTracker
{
public:
	CommonSetting commonSetting;
	ObjectTrackSetting objectTrackSetting;

public:
	// �R���X�g���N�^
	ObjectTracker(CommonSetting _commonSetting, ObjectTrackSetting _objectTrackSetting)
	{
		commonSetting = _commonSetting;
		objectTrackSetting = _objectTrackSetting;
	};

	// ���̒ǐՊJ�n
	void startTracking();


private:
	//�p�^�[���}�b�`���O
	TargetInfo trackPatternMatching(cv::VideoCapture cap);
	void trackPatternMatchingInternal(cv::Mat &frame, cv::Mat &targetImage);
	void showXYPosition(cv::Mat& frame, cv::Point& pt);

	// ���̒ǐ�
	void trackObject(cv::VideoCapture cap);
	void update(cv::Ptr<cv::Tracker> tracker, cv::Mat& frame, cv::Rect2d& roi, cv::Scalar& colorkcf);

	// ����ۑ�
	cv::VideoWriter startWriteVideo(cv::Size size);
	void writeVideo(cv::VideoWriter writer, cv::Mat& frame);

};

class TargetInfo
{
public:
	cv::Mat frame;
	cv::Rect2d roi;
};

