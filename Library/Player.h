#pragma once

//必要なライブラリファイルのロード
#include "Main.h"
#include "CObj.h" 
#include "Obb.h"
#include "MainCamera.h"

#define POWER_TIME 100

//プレイヤークラス
class CPlayer:public CObj3D
{
public:
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
	D3DXVECTOR3 m_vMove;
	D3DXVECTOR3 m_vLastMove;
	OBB_Data m_Obb;
	float m_fSpeed;//移動スピード

	bool m_bHit;//ヒットテスト
};

