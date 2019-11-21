#include "../Library/LibraryHeader.h"
#include "GameHeader.h"

#include "Test.h"
//初期化
void CSceneMain::Init()
{
	//素材読み込み
	LoadTexture();
	LoadAudio();
	LoadMesh();
	
	//カメラ
	CMainCamera* pCamera = new CMainCamera();
	g_Task.InsertObj(pCamera, ObjName::ObjMainCamera);

	//プレイヤー
	D3DXVECTOR3 vPos(0.0f, 0.0f, -20.0f);
	D3DXVECTOR3 vAngle(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vScale(1.0f, 1.0f, 1.0f);

	CPlayer* pPlayer=new CPlayer(vPos, vAngle, vScale);
	g_Task.InsertObj(pPlayer,ObjName::ObjPlayer);

	//2DOBJ
	C2DPolygon* p2D=new C2DPolygon();
	g_Task.InsertObj(p2D, ObjName::ObjDraw2D);

	//敵
	vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	CEnemy* pEnemy=new CEnemy(vPos, vAngle, vScale);
	g_Task.InsertObj(pEnemy, ObjName::ObjEnemy);

	//テスト

	vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vScale = D3DXVECTOR3(2.0f, 2.0f, 2.0f);

	CTest* pTest = new CTest(vPos, vAngle, vScale);
	g_Task.InsertObj(pTest, ObjName::ObjTest);

	//シェーダーにカメラセット
	SHADER->SetCamera(dynamic_cast<CCameraBase*>(pCamera));
	SKIN_SHADER->SetCamera(dynamic_cast<CCameraBase*>(pCamera));
	DRAW->SetCamera(dynamic_cast<CCameraBase*>(pCamera));

	AUDIO->StartMusic(0);
}

//更新
void CSceneMain::Update()
{
	g_Task.Update();
	AUDIO->Update();
}

//描画
void CSceneMain::Draw()
{
	g_Task.Draw();
}

//開放
void CSceneMain::Release()
{
	g_Task.Release();
	AUDIO->DeleteMusic();
	LOADER->Release();
	RAY->Release();	
	TEXTURE->Release();
	MODEL->Release();
}

//音楽
void CSceneMain::LoadAudio()
{
	AUDIO->Load(0, "Audio/King.wav", true);
}

//テクスチャ
void CSceneMain::LoadTexture()
{
	TEXTURE->Load(0, "Texture/Test.png",512,512);
}

//メッシュ
void CSceneMain::LoadMesh()
{
	//プレイヤー
	MODEL->LoadModel(ModelName::ModelPlayer, "Model/Player/Player.obj");

	//球
	MODEL->LoadModel(ModelName::ModelSphere, "Model/Sphere/Sphere.obj");
	
	//照準
	MODEL->LoadModel(ModelName::ModelAiming, "Model/Aiming/Aiming.obj");

	//的
	MODEL->LoadModel(ModelName::ModelTarget, "Model/Target/Target.obj");

}