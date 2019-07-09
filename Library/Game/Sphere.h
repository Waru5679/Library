#pragma once

//ライブラリファイル
#include "../Library/LibraryHeader.h"
#include "../Library/CObj.h"

//球クラス
class CSphere:public CObj3DBase
{
public:
	//コンストラクタ
	CSphere(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale);

	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画
private:
	float m_fRadius;	//球の半径

	CollisionData m_Collision;//当たり判定用
};