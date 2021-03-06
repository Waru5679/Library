//ライブラリファイル
#include "../../Library/LibraryHeader.h"

//ゲームファイル
#include "../GameHeader.h"

//初期化
void CMainCamera::Init()
{
	//初期カメラの位置
	m_vEye = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
	m_vLook = D3DXVECTOR3(0.0f, 2.0f, 1.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//行列初期化
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	//カメラ距離
	m_fCameraDis = 3.0f;
	
	//注視点距離
	m_fLookDis = 3.0f;

	//角度
	m_vAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//更新関数
void CMainCamera::Update()
{
	//回転
	if (INPUT->GetKeyPush(VK_LEFT) == true)
	{
		m_vAngle.y -= 0.05f;
	}

	if (INPUT->GetKeyPush(VK_RIGHT) == true)
	{
		m_vAngle.y += 0.05f;
	}

	//プレイヤーポインタ
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(g_Task.GetObj(ObjName::ObjPlayer));
	m_vPlayerPos = pPlayer->GetPos();
	
	//プレイヤーの方向
	m_matRot = MakeMatRot(m_vAngle);
	D3DXVec3TransformCoord(&m_vPlayerFront, & D3DXVECTOR3(0.0f, 0.0f, 1.0f), &m_matRot);

	//プレイヤーの位置をもとに注視点とカメラ位置を求める
	m_vLook = m_vPlayerPos + m_vPlayerFront * m_fLookDis;
	m_vEye = m_vPlayerPos - m_vPlayerFront * m_fCameraDis;

	m_vEye.y += 2.0f;

	//頂点シェーダ用のマトリックス作成
	CreateVSMatrix(&m_vEye, &m_vLook, &m_vUp, &m_matView, &m_matProj, WINDOW_WIDTH, WINDOW_HEIGHT);
}
