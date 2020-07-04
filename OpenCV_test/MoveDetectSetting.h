#pragma once
#include <string>
class MoveDetectSetting
{
public:
	enum class EMoveDetectInputType {
		E_MOVE_DETECT_INPUT_TYPE_CAMERA,		// �J����
		E_MOVE_DETECT_INPUT_TYPE_FILE			// ����t�@�C��
	};

public:
	// �}�X�N�T�C�Y
	int maskSize = 5;
	// ��������ƌ��m����臒l
	int moveDetectThresholdSize = 100;

	// input���
	EMoveDetectInputType inputType = EMoveDetectInputType::E_MOVE_DETECT_INPUT_TYPE_CAMERA;

	// ����t�@�C���p�X
	std::string inputFilePath = ".\\contents\\video\\vtest.avi";

	// �J�������
	int inputCameraDeviceNo = 0;

public:
	MoveDetectSetting() {};

	//// �}�X�N�T�C�Y���擾
	//int getMaskSize() { return maskSize; };
	//// ��������ƌ��m����臒l���擾
	//int getDetectThresholdSize() { return moveDetectThresholdSize; };

};
