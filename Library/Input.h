#pragma once

//入出力に必要なヘッダー
#include <Windows.h>

//入力クラス
class CInput
{
public:
	void Init();//初期化
			
	//マウス位置取得　
	void SetMousPos(UINT* uMsg, LPARAM* lParam);

	//キー・マウスボタンのプッシュ確認
	bool GetKeyPush(int key);			//通常
	bool GetTriggerKeyPush(int key);	//トリガー式

	//マウスの位置情報取得
	int GetMousPosX(){return m_mous_x;}
	int GetMousPosY(){return m_mous_y;}
private:
	int m_mous_x;	//マウスの位置X
	int m_mous_y;	//マウスの位置Y
	bool m_key[256];//キーの状態を保存する
};

extern CInput g_input;