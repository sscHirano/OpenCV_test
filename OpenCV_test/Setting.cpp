#include "Setting.h"

#include <opencv2/tracking/tracker.hpp>

// ファイルから設定を読み込んで返却する
// 今は、固定返却する
// .hで初期値を設定しているので、上書きしたい場合はこちらを変更する
Setting Setting::readSettingFile()
{
	// 共通設定
	Setting setting;
	//※デフォルト値を設定済みのため、割愛
	//setting.activeMode = EActiveMode::E_MOVE_OBJECT_DETECTER_MODE;
	setting.activeMode = EActiveMode::E_OBJECT_TRACKER_MODE;

	// 入力元
	//inputType = EInputType::E_INPUT_TYPE_CAMERA;
	setting.commonSetting.inputType = CommonSetting::EInputType::E_INPUT_TYPE_FILE;
	// カメラ番号
	setting.commonSetting.inputCameraDeviceNo = 0;
	// 動画ファイルパス(input種別がE_INPUT_TYPE_FILEの場合のみ有効)
	setting.commonSetting.inputFilePath = "C:\\work\\OpenCV\\contents\\PANTO CAMERA 2.mp4";

	// 動体検知設定
	//パターンマッチング
	//setting.objectTrackSetting.matchMeathod = CV_TM_SQDIFF;
	setting.objectTrackSetting.matchMeathod = CV_TM_SQDIFF_NORMED;	//デフォルト
	//setting.objectTrackSetting.matchMeathod = CV_TM_CCORR;
	//setting.objectTrackSetting.matchMeathod = CV_TM_CCORR_NORMED;
	//setting.objectTrackSetting.matchMeathod = CV_TM_CCOEFF;
	//setting.objectTrackSetting.matchMeathod = CV_TM_CCOEFF_NORMED;
	//パターンマッチング閾値
	setting.objectTrackSetting.matchThreashold = 0.2;

	// 物体追跡設定
	setting.objectTrackSetting.enableTrackKcf = true;
	setting.objectTrackSetting.enableTrackTld = false;
	setting.objectTrackSetting.enableTrackMedianFlow = false;
	setting.objectTrackSetting.enableTrackBoosting = false;
	setting.objectTrackSetting.enableTrackMil = false;

	setting.objectTrackSetting.trackingMode = ObjectTrackSetting::ETrackingMode::E_PATARN_MATCH;	//パターンマッチング
//	setting.objectTrackSetting.trackingMode = ObjectTrackSetting::ETrackingMode::E_OBJECT_TRACK;	//トラッキング

	// 物体追跡 動画保存
	setting.objectTrackSetting.enableSaveVideo = false;
	setting.objectTrackSetting.saveVideoPath = "C:\\work\\OpenCV\\contents\\output\\save_video.avi";

	return setting;
}