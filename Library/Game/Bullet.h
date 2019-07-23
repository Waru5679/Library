#pragma once
#include "../Library/CObj.h"

//弾クラス
class CBullet:public CObj3DBase
{
public:
	//コンストラクタ
	CBullet(D3DXVECTOR3 Pos,D3DXVECTOR3 Angle,D3DXVECTOR3 Scale,D3DXVECTOR3 Dir);
	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画
private:
	float m_fSpeed;			//移動速度
	D3DXVECTOR3 m_vMove;	//移動ベクトル
	int m_Count;			//カウンター
};