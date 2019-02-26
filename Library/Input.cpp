#include "Input.h"

CInput g_input;

void CInput::Init()
{
	m_mous_x = 0;
	m_mous_y = 0;

	for (int i = 0; i < 256; i++)
		m_key[i] = false;
}

//マウス位置取得用　ウィンドウプロジージャー内に宣言
void CInput::SetMousPos(UINT* uMsg, LPARAM* lParam)
{
	switch (*uMsg)
	{
		case WM_MOUSEMOVE:	//マウス操作したとき
		{
			POINT point = { LOWORD(*lParam),HIWORD((*lParam)) };

			m_mous_x = point.x;	//カーソルのx座標
			m_mous_y = point.y;	//カーソルのy座標
		}
		break;
	}
}

//キー・マウスボタンのプッシュ確認
//ボタンが押されていればtrue,押されていなければfalseを返す
bool CInput::GetKeyPush(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		m_key[key] = true;
		return true;
	}
	m_key[key] = false;
	return false;
}

//キー・マウスボタンのプッシュ確認 (トリガー式)
//新しき押されたならtrue,
//すでに押されていた　または　押されていないなら false　
bool CInput::GetTriggerKeyPush(int key)
{
	//キーが押されていてる場合
	if ( (GetAsyncKeyState(key) & 0x8000) ) 
	{
		//キーのフラグがオフなら
		if (m_key[key] == false)
		{
			m_key[key] = true;
			return true;
		}
	}
	//キーが押されていない場合
	else
	{
		m_key[key] = false;
	}
	return false;
}