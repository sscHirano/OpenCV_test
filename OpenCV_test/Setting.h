#pragma once
class Setting
{
	/******************
		公開定数
	*******************/
public:
	enum class EActiveMode {
		E_MOVE_OBJECT_DETECTER_MODE,		// 動体検知
		E_OBJECT_TRACKER_MODE				// 物体追跡
	};


	/******************
		公開関数
	*******************/
public:
	static Setting* readSettingFile();
	EActiveMode getActiveMode()
	{
		return activeMode;
	};

	/******************
		ローカル変数
	*******************/
private:
	EActiveMode activeMode = EActiveMode::E_OBJECT_TRACKER_MODE;

	/******************
		内部関数
	*******************/
private:
	// readSettingFile()を使ってインスタンス生成するため、非公開
	Setting() {
	};

};

