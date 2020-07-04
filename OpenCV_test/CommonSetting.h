#pragma once

#include <string>

class CommonSetting
{
public:
	enum class EInputType {
		E_INPUT_TYPE_CAMERA,		// �J����
		E_INPUT_TYPE_FILE			// ����t�@�C��
	};
	// input���
	EInputType inputType = EInputType::E_INPUT_TYPE_CAMERA;

	// ����t�@�C���p�X(input��ʂ�E_INPUT_TYPE_FILE�̏ꍇ�̂ݗL��)
	std::string inputFilePath = ".\\contents\\video\\vtest.avi";

	// �J�����f�o�C�X�ԍ�(input��ʂ�E_INPUT_TYPE_CAMERA�̏ꍇ�̂ݗL��)
	int inputCameraDeviceNo = 0;
};

