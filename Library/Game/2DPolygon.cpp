//ライブラリファイル
#include "../Library/LibraryHeader.h"

//ゲームファイル
#include "../Library/GameHeader.h"

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
	m_Color = ColorData(1.0f, 1.0f, 1.0f, 0.5f);
}

//更新
void C2DPolygon::Update()
{
	//移動テスト
	if (INPUT->GetKeyPush(VK_RIGHT)==true)
	{
		m_Move.x = 1.0f;
	}
	if (INPUT->GetKeyPush(VK_LEFT) == true)
	{
		m_Move.x = -1.0f;
	}
	if (INPUT->GetKeyPush(VK_UP) == true)
	{
		m_Move.y = -1.0f;
	}
	if (INPUT->GetKeyPush(VK_DOWN) == true)
	{
		m_Move.y = 1.0f;
	}

	//回転テスト
	if (INPUT->GetKeyPush(VK_RETURN) == true)
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
	m_Src=RECT_F(200.0f, 200.0f, 56.0f, 56.0f);
	
	//描画位置
	m_Dst = RECT_F(m_ScreenPos.y, m_ScreenPos.x, 100.0f, 100.0f);
	
	//描画
	DRAW->DrawTexture(0, &m_Src, &m_Dst, &m_Color, m_fRad);
}