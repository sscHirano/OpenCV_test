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
	// コンストラクタ
	ObjectTracker(CommonSetting _commonSetting, ObjectTrackSetting _objectTrackSetting)
	{
		commonSetting = _commonSetting;
		objectTrackSetting = _objectTrackSetting;
	};

	// 物体追跡開始
	void startTracking();


private:
	TargetInfo searchTargetObject(cv::VideoCapture cap);
	void searchTargetInternal(cv::Mat frame, cv::Mat targetImage);
	void update(cv::Ptr<cv::Tracker> tracker, cv::Mat& frame, cv::Rect2d& roi, cv::Scalar& colorkcf);
};

class TargetInfo
{
public:
	cv::Mat frame;
	cv::Rect2d roi;
};

