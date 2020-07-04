#pragma once

class MoveDetectSetting
{
public:


	/************
	調整オプション
	**************/
// マスクサイズ
	int maskSize = 5;
	// 動きありと検知する閾値
	int moveDetectThresholdSize = 100;


public:
	MoveDetectSetting() {};

	//// マスクサイズを取得
	//int getMaskSize() { return maskSize; };
	//// 動きありと検知する閾値を取得
	//int getDetectThresholdSize() { return moveDetectThresholdSize; };

};
