#include <math.h>

//ライブラリファイル
#include "../Library/Math.h"
#include "../Library/Task.h"
#include "../Library/ObjLoader.h"
#include "../Library/Camera.h"
#include "../Library/Font.h"
#include "../Library/Ray.h"

//ゲームファイル
#include "RayTest.h"

//コンストラクタ
CRayTest::CRayTest(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale)
{
	m_vPos = Pos;
	m_vAngle = Angle;
	m_vScale = Scale;
}

//初期化
void CRayTest::Init()
{
	//行列初期化
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matWorld);

	//モデル
	m_pMesh = g_Loader.GetMesh(ModelName::ModelRayTest);

	g_Ray.Insert(this);
}

//更新
void CRayTest::Update()
{
	//ワールド行列作成
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

//描画
void CRayTest::Draw()
{
	g_Loader.Draw(m_matWorld, m_pMesh,NULL);
}
