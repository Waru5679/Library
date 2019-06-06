#pragma once

//ライブラリファイル
#include "../Library/Main.h"
#include "../Library/CObj.h" 

//Rayのテストクラス
class CRayTest :public CObj3DBase
{
public:
	//コンストラクタ
	CRayTest(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale);
	~CRayTest() {};

	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画

	D3DXVECTOR3 GetPos() { return m_vPos; }
	D3DXVECTOR3 GetAngle() { return m_vAngle; }
	
private:
};

#pragma once
