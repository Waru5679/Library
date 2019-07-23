#pragma once

//ライブラリファイル
#include "../Library/CObj.h"
#include "../Library/LibraryHeader.h"

//ゲームファイル
#include "MainCamera.h"

//プレイヤークラス
class CPlayer:public CObj3DBase
{
public:
	//コンストラクタ
	CPlayer(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale);
	~CPlayer() {};

	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画
	void Input();	//入力
	void Release();	//開放

	D3DXVECTOR3 GetPos() { return m_vPos; }
	D3DXVECTOR3 GetAngle() { return m_vAngle; }
	D3DXVECTOR3 GetDir() { return m_vDir; }
	D3DXVECTOR3 GetLastMove() { return m_vLastMove; }
	float GetAimDis() { return m_fAimDis; }
	
private:
	D3DXVECTOR3 m_vMove;		//移動ベクトル
	D3DXVECTOR3 m_vDir;			//向き
	D3DXVECTOR3 m_vLastMove;	//最後に移動した向き
	CCollisionData m_Collision;	//当たり判定	
	float m_fSpeed;				//移動スピード
	float m_fAimDis;			//照準との距離
	bool m_bHit;				//ヒットテスト
};