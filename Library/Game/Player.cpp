#pragma once
//ライブラリファイル
#include "../Library/LibraryHeader.h"

//ゲームファイル
#include  "GameHeader.h"

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
	m_pMesh = LOADER->GetMesh(ModelName::ModelPlayer);
	
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
	m_fAimDis = 5.0f;

	//照準生成
	CAiming* pAiming = new CAiming(m_vPos + m_vDir * m_fAimDis, m_vAngle, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	g_Task.InsertObj(pAiming, ObjName::ObjAiming);
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
	//向きを更新する
	if (INPUT->GetKeyPush(VK_LEFT) == true)
	{
		m_vAngle.y -= 0.05f;
		//m_vDir+=
	}

	if (INPUT->GetKeyPush(VK_RIGHT) == true)
	{
		m_vAngle.y += 0.05f;
		//m_vDir+=
	}
}

//描画
void CPlayer::Draw()
{
	if(m_bHit==true)
		FONT->DrawStr(L"Hit", 200.0f, 20.0f, 32.0f, 0.0f);

	LOADER->Draw(m_matWorld, m_pMesh,NULL);
}
