//ライブラリファイル
#include "../Library/Font.h"
#include "../Library/Input.h"

//ゲームファイル
#include "2DPolygon.h"

//初期化
void C2DPolygon::Init()
{
	//位置
	m_ScreenPos.x = 0.0f;
	m_ScreenPos.y = 0.0f;
	
	//移動
	m_Move.x = 0.0f;
	m_Move.y = 0.0f;

	//回転
	m_fRad = 0.0f;

	//描画色
	m_fColor[0] = 1.0f;
	m_fColor[1] = 1.0f;
	m_fColor[2] = 1.0f;
	m_fColor[3] = 0.5f;

}

//更新
void C2DPolygon::Update()
{
	//移動テスト
	if (g_input.GetKeyPush(VK_RIGHT)==true)
	{
		m_Move.x = 1.0f;
	}
	if (g_input.GetKeyPush(VK_LEFT) == true)
	{
		m_Move.x = -1.0f;
	}
	if (g_input.GetKeyPush(VK_UP) == true)
	{
		m_Move.y = -1.0f;
	}
	if (g_input.GetKeyPush(VK_DOWN) == true)
	{
		m_Move.y = 1.0f;
	}

	//回転テスト
	if (g_input.GetKeyPush(VK_RETURN) == true)
	{
		m_fRad += 1.0f;
	}

	//移動
	m_ScreenPos.x += m_Move.x;
	m_ScreenPos.y += m_Move.y;

	//移動初期化
	m_Move.x = 0.0f;
	m_Move.y = 0.0f;
}

//描画
void C2DPolygon::Draw()
{
	//切り取り位置
	m_Src.m_top =200.0f;
	m_Src.m_left = 200.0f;
	m_Src.m_right = m_Src.m_left + 56.0f;
	m_Src.m_bottom = m_Src.m_top + 56.0f;
	
	//描画位置
	m_Dst.m_top = m_ScreenPos.y;
	m_Dst.m_left = m_ScreenPos.x;
	m_Dst.m_right = m_Dst.m_left + 100.0f;
	m_Dst.m_bottom = m_Dst.m_top + 100.0f;
	
	//描画
	g_Draw.Draw2D(0, &m_Src, &m_Dst, m_fColor, m_fRad);
}