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
		���J�֐�
	*******************/
public:
	static Setting* readSettingFile();
	EActiveMode getActiveMode()
	{
		return activeMode;
	};

	/******************
		���[�J���ϐ�
	*******************/
private:
	EActiveMode activeMode = EActiveMode::E_OBJECT_TRACKER_MODE;

	/******************
		�����֐�
	*******************/
private:
	// readSettingFile()���g���ăC���X�^���X�������邽�߁A����J
	Setting() {
	};

};

