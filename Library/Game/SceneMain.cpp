#include "../Library/LibraryHeader.h"
#include "../Library/GameHeader.h"

//初期化
void CSceneMain::Init()
{
	//素材読み込み
	LoadTexture();
	LoadAudio();
	LoadMesh();
	
	//プレイヤー
	D3DXVECTOR3 vPos(0.0f, 0.0f, -20.0f);
	D3DXVECTOR3 vAngle(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vScale(1.0f, 1.0f, 1.0f);

	CPlayer* pPlayer=new CPlayer(vPos, vAngle, vScale);
	g_Task.InsertObj(pPlayer,ObjName::ObjPlayer);

	//カメラ
	CMainCamera* pCamera = new CMainCamera();
	g_Task.InsertObj(pCamera, ObjName::ObjMainCamera);

	////2DOBJ
	//C2DPolygon* p2D=new C2DPolygon();
	//g_Task.InsertObj(p2D, ObjName::ObjDraw2D);

	////敵
	//vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//vAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//CEnemy* pEnemy=new CEnemy(vPos, vAngle, vScale);
	//g_Task.InsertObj(pEnemy, ObjName::ObjEnemy);

	//シェーダーにカメラセット
	SHADER->SetCamera(dynamic_cast<CameraBase*>(pCamera));
	DRAW->SetCamera(dynamic_cast<CameraBase*>(pCamera));
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

void CSceneMain::Release()
{
	g_Task.Release();
	AUDIO->DeleteMusic();
	LOADER->Release();
	RAY->Release();	
}

//音楽
void CSceneMain::LoadAudio()
{

}

//テクスチャ
void CSceneMain::LoadTexture()
{
	DRAW->LoadTexture(0, "Texture/Test.png",512,512);
}

//メッシュ
void CSceneMain::LoadMesh()
{
	//プレイヤー
	LOADER->LoadMesh(ModelName::ModelPlayer, "Model/Player/Player.obj");

	//球
	LOADER->LoadMesh(ModelName::ModelSphere, "Model/Sphere/Sphere.obj");
}