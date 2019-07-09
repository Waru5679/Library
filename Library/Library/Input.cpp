#include "Input.h"
#include "Release.h"

//インスタンス
CInput* CInput::m_pInstance = nullptr;

//初期化
void CInput::Init()
{
	m_Mous_x = 0;
	m_Mous_y = 0;

	//キー情報初期化
	for (int i = 0; i < KEY_BOARD_ARRAY_SIZE; i++)
		m_Key[i] = false;
}

//マウス位置取得
void CInput::SetMousPos(UINT* uMsg, LPARAM* lParam)
{
	switch (*uMsg)
	{
		case WM_MOUSEMOVE:	//マウス操作したとき
		{
			POINT point = { LOWORD(*lParam),HIWORD((*lParam)) };

			m_Mous_x = point.x;	//カーソルのx座標
			m_Mous_y = point.y;	//カーソルのy座標
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
		m_Key[key] = true;
		return true;
	}
	m_Key[key] = false;
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
		if (m_Key[key] == false)
		{
			m_Key[key] = true;
			return true;
		}
	}
	//キーが押されていない場合
	else
	{
		m_Key[key] = false;
	}
	return false;
}

//解放
void CInput::Release()
{
	//インスタンス破棄
	PointerRelease(m_pInstance);
}