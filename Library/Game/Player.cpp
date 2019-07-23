#pragma once
//ライブラリファイル
#include "../Library/LibraryHeader.h"

//ゲームファイル
#include "GameHeader.h"
#include <math.h>

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
	m_pMesh = MODEL->GetModelData(ModelName::ModelPlayer);
	
	//移動ベクトル
	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//向き
	m_vDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//移動スピード
	m_fSpeed=0.2f;
		
	//ヒットテスト
	m_bHit=false;

	//当たり判定
	m_Collision = HIT->CollisionCreate(this);
	HIT->Insert(&m_Collision);
	
	//Ray登録
	RAY->Insert(this);

	//照準の距離
	m_fAimDis = 10.0f;

	//照準生成
	CAiming* pAiming = new CAiming(m_vPos + m_vDir * m_fAimDis, m_vAngle, D3DXVECTOR3(3.0f, 3.0f, 3.0f));
	g_Task.InsertObj(pAiming, ObjName::ObjAiming);
	pAiming = nullptr;
}

//更新
void CPlayer::Update()
{
	//入力
	Input();

	//移動の正規化
	D3DXVec3Normalize(&m_vMove, &m_vMove);

	//カメラ取得
	CMainCamera*pCamera = dynamic_cast<CMainCamera*> (g_Task.GetObj(ObjName::ObjMainCamera));

	//カメラの向きから移動ベクトルを変換
	m_matRot = MakeMatRot(pCamera->GetAngle());
	D3DXVec3TransformCoord(&m_vMove, &m_vMove, &m_matRot);

	//移動
	m_vPos += m_vMove*m_fSpeed;

	//最後の移動を保存
	if(m_vMove!=D3DXVECTOR3(0.0f,0.0f,0.0f))
		m_vLastMove = m_vMove;

	//当たり判定データ更新
	HIT->UpData(&m_Collision);

	//ヒット確認
	if(HIT->Hit()==true)
	{
		m_bHit = true;
	}
	else
	{
		m_bHit = false;
	}
	
	//ワールド行列作成
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);

	//移動初期化
	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//カメラ更新
	pCamera->Update();

	//カメラポインタ解放
	pCamera = nullptr;
}

//入力
void CPlayer::Input()
{
	//移動
	if (INPUT->GetKeyPush('W') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
	if (INPUT->GetKeyPush('S') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}
	if (INPUT->GetKeyPush('D') == true)
	{
		m_vMove += D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	}
	if (INPUT->GetKeyPush('A') == true)
	{
		m_vMove += D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	}
	if (INPUT->GetKeyPush('E') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	if (INPUT->GetKeyPush('Q') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	}

	//回転
	if (INPUT->GetKeyPush(VK_LEFT) == true)
	{
		m_vAngle.y -= 0.05f;
	}

	if (INPUT->GetKeyPush(VK_RIGHT) == true)
	{
		m_vAngle.y += 0.05f;
	}

	//攻撃
	if (INPUT->GetTriggerKeyPush(VK_SPACE))
	{
		//エイムポインタ
		CAiming* pAiming = dynamic_cast<CAiming*>(g_Task.GetObj(ObjName::ObjAiming));
		
		//弾の移動方向
		D3DXVECTOR3 vDir;
		if (pAiming != nullptr)
		{
			vDir = pAiming->GetPos() - m_vPos;
			D3DXVec3Normalize(&vDir, &vDir);
		}

		//生成
		CBullet* pBullet = new CBullet(m_vPos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), vDir);
		g_Task.InsertObj(pBullet, ObjName::ObjBullet);

		//ポインタ解放
		pAiming = nullptr;
		pBullet = nullptr;
	}
}

//描画
void CPlayer::Draw()
{
	MODEL->Draw(m_matWorld, m_pMesh,NULL);
}

//開放
void CPlayer::Release()
{
	//メッシュデータ解放
	m_pMesh->Release();
	m_pMesh = nullptr;

	//当たり判定解放
	m_Collision.Release();
}
