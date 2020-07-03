#pragma once
class Setting
{
	/******************
		���J�萔
	*******************/
public:
	enum class EActiveMode {
		E_MOVE_OBJECT_DETECTER_MODE,		// ���̌��m
		E_OBJECT_TRACKER_MODE				// ���̒ǐ�
	};

	/******************
		���[�J���ϐ�
	*******************/
private:
	EActiveMode activeMode = EActiveMode::E_OBJECT_TRACKER_MODE;

	/******************
		���J�֐�
	*******************/
public:
	static Setting* readSettingFile();

	/******************
		�����֐�
	*******************/
private:
	// readSettingFile()���g���ăC���X�^���X�������邽�߁A����J
	Setting() {
	};

};

