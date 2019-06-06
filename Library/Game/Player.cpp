#include <math.h>

//ライブラリファイル
#include "../Library/Math.h"
#include "../Library/Task.h"
#include "../Library/ObjLoader.h"
#include "../Library/Input.h"
#include "../Library/Camera.h"
#include "../Library/Font.h"
#include "../Library/Ray.h"
#include "../Library/Hit.h"

//ゲームファイル
#include "Player.h"
#include "MainCamera.h"

//コンストラクタ
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
	m_pMesh = g_Loader.GetMesh(ModelName::ModelSphere);
	
	//移動ベクトル
	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//移動スピード
	m_fSpeed=0.2f;

	//当たり判定セット
	m_Obb = g_Obb.SetOBB(m_vPos, m_vAngle, m_vScale, m_pMesh->vMin, m_pMesh->vMax, m_id, this);
	g_Obb.Insert(&m_Obb);

	//ヒットテスト
	m_bHit=false;

	//半径求める
	D3DXVECTOR3 vMax, vMin;
	vMax = m_pMesh->vMax;
	vMin = m_pMesh->vMin;

	//一番長い距離から半径を設定
	float Dia = MostLongComponent(vMax - vMin);
	Dia = fabsf(Dia);
	float radius = Dia / 2.0f;

	//登録
	m_SphereData = g_Hit.Create(m_vPos, radius,m_vScale);
	g_Hit.Insert(&m_SphereData);
}

//更新
void CPlayer::Update()
{
	//入力
	Input();

	//正規化
	D3DXVec3Normalize(&m_vMove, &m_vMove);

	//カメラ取得
	CMainCamera* m_pCamera=dynamic_cast<CMainCamera*> (g_Task.GetObj(ObjName::ObjMainCamera));

	//カメラの向きから移動ベクトルを変換
	m_matRot = MakeMatRot(m_pCamera->GetAngle());
	D3DXVec3TransformCoord(&m_vMove, &m_vMove, &m_matRot);

	//移動
	m_vPos += m_vMove*m_fSpeed;

	//最後の移動を保存
	if(m_vMove!=D3DXVECTOR3(0.0f,0.0f,0.0f))
		m_vLastMove = m_vMove;
	
	//当たり判定更新
	g_Obb.Update(&m_Obb, m_vPos, m_vAngle, m_vScale, m_pMesh->vMin, m_pMesh->vMax);

	//球データ更新
	g_Hit.UpData(&m_SphereData, m_vPos);
	
	D3DXVECTOR3 vShear;
	if(g_Hit.SphereHit()==true)
	//if (g_Ray.RayHit(&vShear,this,m_vLastMove*m_fSpeed,ObjRayTest) == true)
	{
		m_bHit = true;

		//移動を戻す
		m_vPos -= m_vMove * m_fSpeed;

		//壁擦り
		//m_vPos += vShear * m_fSpeed;
	}
	else
	{
		m_bHit = false;
	}

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
	if (g_input.GetKeyPush('Y') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	if (g_input.GetKeyPush('H') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	}


	//回転
	//移動
	if (g_input.GetKeyPush('J') == true)
	{
		m_vAngle += D3DXVECTOR3(0.0f, 0.2f, 0.0f);
	}
	if (g_input.GetKeyPush('L') == true)
	{
		m_vAngle += D3DXVECTOR3(0.0f, -0.2f, 0.0f);
	}
	if (g_input.GetKeyPush('I') == true)
	{
		m_vAngle += D3DXVECTOR3(0.2f, 0.0f, 0.0f);
	}
	if (g_input.GetKeyPush('K') == true)
	{
		m_vAngle += D3DXVECTOR3(-0.2f, 0.0f, 0.0f);
	}
	if (g_input.GetKeyPush('U') == true)
	{
		m_vAngle += D3DXVECTOR3(0.0f, 0.0f, 0.2f);
	}
	if (g_input.GetKeyPush('O') == true)
	{
		m_vAngle += D3DXVECTOR3(0.0f, 0.0f, -0.2f);
	}
}

//描画
void CPlayer::Draw()
{
	if(m_bHit==true)
	g_Font.DrawStr(L"Hit", 200.0f, 20.0f, 32.0f, 0.0f);

	g_Loader.Draw(m_matWorld, m_pMesh,NULL);
}
