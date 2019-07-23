#include "Aiming.h"
#include "GameHeader.h"

//コンストラクタ
CAiming::CAiming(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale)
{
	m_vPos = vPos;
	m_vAngle = vAngle;
	m_vScale = vScale;
}

//初期化
void CAiming::Init()
{
	//行列初期化
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);

	//モデル
	m_pMesh = MODEL->GetModelData(ModelAiming);

	//ワールド行列
	m_matWorld=MakeMatWorld(m_vPos, m_vAngle, m_vScale);

	//プレイヤーのポインタを取得
	m_pPlayer = dynamic_cast<CPlayer*>(g_Task.GetObj(ObjName::ObjPlayer));

	//カメラのポインタ
	m_pCamera = dynamic_cast<CMainCamera*>(g_Task.GetObj(ObjName::ObjMainCamera));

	//移動ベクトル
	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//更新
void CAiming::Update()
{
	//プレイヤーのポインタを取得
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = dynamic_cast<CPlayer*>(g_Task.GetObj(ObjName::ObjPlayer));
	}

	//カメラのポインタ取得
	if (m_pCamera == nullptr)
	{
		m_pCamera = dynamic_cast<CMainCamera*>(g_Task.GetObj(ObjName::ObjMainCamera));
	}

	//カメラ→プレイヤーのベクトル
	D3DXVECTOR3 vDir;
	vDir = m_pPlayer->GetPos()- m_pCamera->GetEye();
	D3DXVec3Normalize(&vDir, &vDir);

	//カメラの回転
	m_vAngle.y = m_pCamera->GetAngle().y;

	//カメラの向きから位置を更新
	m_vMove = vDir * m_pPlayer->GetAimDis();
	m_vMove.y = 0.0f;

	m_vPos = m_pPlayer->GetPos() + m_vMove;

	//ワールド行列
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

//描画
void CAiming::Draw()
{
	MODEL->Draw(m_matWorld, m_pMesh, nullptr);
}

//開放
void CAiming::Release()
{
}