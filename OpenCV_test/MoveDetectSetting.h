#pragma once

class MoveDetectSetting
{
public:


	/************
	�����I�v�V����
	**************/
// �}�X�N�T�C�Y
	int maskSize = 5;
	// ��������ƌ��m����臒l
	int moveDetectThresholdSize = 100;


public:
	MoveDetectSetting() {};

	//// �}�X�N�T�C�Y���擾
	//int getMaskSize() { return maskSize; };
	//// ��������ƌ��m����臒l���擾
	//int getDetectThresholdSize() { return moveDetectThresholdSize; };

};
