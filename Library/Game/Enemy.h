#pragma once

//ライブラリファイル
#include "../Library/Header.h"

constexpr int ATTACK_TIME{ 300 };	//攻撃の間隔
constexpr int ATTACK_NUM_Y{ 10 };	//攻撃の数
constexpr int ATTACK_NUM_X{ 10 };	//攻撃の数

constexpr float ATTACK_RAD_X{ 360.0f / ATTACK_NUM_X };	//攻撃の角度
constexpr float ATTACK_RAD_Y{ 360.0f / ATTACK_NUM_Y };	//攻撃の角度

//敵クラス
class CEnemy :public CObj3DBase
{
public:
	//コンストラクタ
	CEnemy(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale);
	~CEnemy() {};

	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画

	D3DXVECTOR3 GetPos() { return m_vPos; }
	D3DXVECTOR3 GetAngle() { return m_vAngle; }

private:
	D3DXVECTOR3 m_vMove;		//移動ベクトル
	CollisionData m_Collision;	//当たり判定	
	
	int m_AttackCount;			//攻撃のカウント
	float m_AttackLength_x;		//攻撃距離
	float m_AttackLength_y;		//攻撃距離

	float m_fSpeed;				//移動スピード
};