#pragma once

//ライブラリファイル
#include "../Library/CObj.h"

//町クラス
class CStreet :public CObj3DBase
{
public:
	//コンストラクタ
	CStreet(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale);
	
	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画
private:
};