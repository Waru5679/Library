#pragma once

//ライブラリファイル
#include "../Library/LibraryHeader.h"

//ゲームファイル
#include "Player.h"

constexpr int DELETE_TIME{ 300 };

//敵の弾クラス
class CEnemyBullet :public CObj3DBase
{
public:
	//コンストラクタ
	CEnemyBullet(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale);
	~CEnemyBullet() {};

	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画
	void Release();	//解放

	D3DXVECTOR3 GetPos() { return m_vPos; }
	D3DXVECTOR3 GetAngle() { return m_vAngle; }
private:
	int m_Count;				//カウンター
	D3DXVECTOR3 m_vMove;		//移動ベクトル
	CollisionData m_Collision;	//当たり判定	
	float m_fSpeed;				//移動スピード
	CPlayer* m_pPlayer;			//プレイヤーのポインタ
	D3DXVECTOR3 m_vPlayerPos;	//プレイヤーの位置
};