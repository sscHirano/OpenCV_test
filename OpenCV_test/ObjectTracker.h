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
	//パターンマッチング
	TargetInfo trackPatternMatching(cv::VideoCapture cap);
	void trackPatternMatchingInternal(cv::Mat &frame, cv::Mat &targetImage);
	void showXYPosition(cv::Mat& frame, cv::Point& pt);

	// 物体追跡
	void trackObject(cv::VideoCapture cap);
	void update(cv::Ptr<cv::Tracker> tracker, cv::Mat& frame, cv::Rect2d& roi, cv::Scalar& colorkcf);

	// 動画保存
	cv::VideoWriter startWriteVideo(cv::Size size);
	void writeVideo(cv::VideoWriter writer, cv::Mat& frame);

};

class TargetInfo
{
public:
	cv::Mat frame;
	cv::Rect2d roi;
};

