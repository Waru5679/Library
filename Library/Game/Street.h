#pragma once

//ライブラリファイル
#include "../Library/CObj.h"
#include "../Library/Obb.h"

//町クラス
class CStreet :public CObj3D
{
public:
	//コンストラクタ
	CStreet(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale);
	
	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画
private:
	OBB_Data m_Obb;	//Obb
};