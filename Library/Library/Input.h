#pragma once

#include <Windows.h>
#include "Singleton.h"

constexpr int KEY_BOARD_ARRAY_SIZE{ 256 };

//入力クラス
class CInput:public CSingleton<CInput>
{
public:
	void Init();	//初期化
	void Release();	//解放
			
	//マウス位置取得　
	void SetMousPos(UINT* uMsg, LPARAM* lParam);

	//キー・マウスボタンのプッシュ確認
	bool GetKeyPush(int key);			//通常
	bool GetTriggerKeyPush(int key);	//トリガー式

	//マウスの位置情報取得
	int GetMousPosX(){return m_Mous_x;}
	int GetMousPosY(){return m_Mous_y;}
private:
	int m_Mous_x;	//マウスの位置X
	int m_Mous_y;	//マウスの位置Y

	bool m_Key[KEY_BOARD_ARRAY_SIZE];//キーの状態を保存する
};