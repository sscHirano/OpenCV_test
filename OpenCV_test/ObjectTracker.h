#pragma once

#include "CommonSetting.h"
#include "ObjectTrackSetting.h"

class ObjectTracker
{
public:
	// コンストラクタ
	ObjectTracker(CommonSetting commonSetting, ObjectTrackSetting objectTrackSetting) {};

	// 物体追跡開始
	void startTracking();

};

