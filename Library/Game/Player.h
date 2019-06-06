#pragma once

//ライブラリファイル
#include "../Library/Main.h"
#include "../Library/CObj.h" 
#include "../Library/Obb.h"
#include "../Library/Hit.h"

//プレイヤークラス
class CPlayer:public CObj3D
{
public:
	//コンストラクタ
	CPlayer(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale);
	~CPlayer() {};

	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画
	void Input();	//入力

	D3DXVECTOR3 GetPos() { return m_vPos; }
	D3DXVECTOR3 GetAngle() { return m_vAngle; }
	D3DXVECTOR3 GetDir() { return m_vLastMove; }
	
private:
	D3DXVECTOR3 m_vMove;	//移動ベクトル
	D3DXVECTOR3 m_vLastMove;//最後に移動した向き
	
	float m_fSpeed;	//移動スピード
	bool m_bHit;	//ヒットテスト

	OBB_Data m_Obb;			//OBBデータ
	SphereData m_SphereData;	//球データ
};

