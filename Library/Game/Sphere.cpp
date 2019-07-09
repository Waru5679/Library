#include "GameHeader.h"
#include <math.h>

//コンストラクタ
CSphere::CSphere(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale)
{
	m_vPos	 = vPos;
	m_vAngle = vAngle;
	m_vScale = vScale;
}

//初期化
void CSphere::Init()
{
	//行列初期化
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matWorld);

	//モデル
	m_pMesh = LOADER->GetMesh(ModelName::ModelSphere);
	   	
	//登録
	m_Collision = HIT->CollisionCreate(this);
	HIT->Insert(&m_Collision);

	//Ray登録
	RAY->Insert(this);

	//ワールド行列作成
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

//更新
void CSphere::Update()
{
	//ワールド行列作成
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);

	HIT->UpData(&m_Collision);
}

//描画
void CSphere::Draw()
{
	LOADER->Draw(m_matWorld, m_pMesh,NULL);
}