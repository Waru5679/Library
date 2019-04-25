#include "Player.h"
#include "Math.h"
#include "Task.h"
#include "ObjLoader.h"
#include "Input.h"
#include "Camera.h"

CPlayer::CPlayer(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale)
{
	m_vPos = Pos;
	m_vAngle = Angle;
	m_vScale = Scale;
}

//初期化
void CPlayer::Init()
{
	//行列初期化
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matWorld);
	
	//モデル
	m_Mesh = g_Task.GetMesh(0);

	//最小値・最大値
	m_vMin = m_Mesh.vMin;
	m_vMax = m_Mesh.vMax;

	//移動ベクトル
	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//移動スピード
	m_fSpeed=0.8f;

	//当たり判定セット
	m_Obb = g_Obb.SetOBB(m_vPos, m_vAngle, m_vScale, m_vMin, m_vMax, m_id, this);
	g_Obb.Insert(&m_Obb);
}

//更新
void CPlayer::Update()
{
	//入力
	Input();

	//正規化
	D3DXVec3Normalize(&m_vMove, &m_vMove);

	//カメラ取得
	CMainCamera* m_pCamera=dynamic_cast<CMainCamera*> (g_Task.GetObj(Obj_MainCamera));

	//カメラの向きから移動ベクトルを変換
	m_matRot = MakeMatRot(m_pCamera->GetAngle());
	D3DXVec3TransformCoord(&m_vMove, &m_vMove, &m_matRot);

	//移動
	m_vPos += m_vMove * m_fSpeed;
	
	//当たり判定更新
	g_Obb.Update(&m_Obb, m_vPos, m_vAngle, m_vScale, m_vMin, m_vMax);
		
	//ワールド行列作成
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);

	//移動初期化
	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//入力
void CPlayer::Input()
{
	//移動
	if (g_input.GetKeyPush('W') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
	if (g_input.GetKeyPush('S') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}
	if (g_input.GetKeyPush('D') == true)
	{
		m_vMove += D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	}
	if (g_input.GetKeyPush('A') == true)
	{
		m_vMove += D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	}
}

#include "Font.h"
//描画
void CPlayer::Draw()
{
	CFont::DrawStr(L"あ",0.0f, 0.0f, 32.0f,m_matWorld);
	//g_Loader.Draw(m_matWorld, &m_Mesh);

	
}
