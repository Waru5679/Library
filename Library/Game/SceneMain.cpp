//ライブラリファイル
#include "../Library/Task.h"
#include "../Library/ObjLoader.h"
#include "../Library/Audio.h"
#include "../Library/Polygon.h"
#include "../Library/Shader.h"

//ゲームファイル
#include "2DPolygon.h"
#include "Street.h"
#include "MainCamera.h"
#include "SceneMain.h"

//Debug
#include "RayTest.h"

//初期化
void CSceneMain::Init()
{
	//素材読み込み
	LoadTexture();
	LoadAudio();
	LoadMesh();
	
	//プレイヤー
	D3DXVECTOR3 vPos(0.0f, 0.0f, -10.0f);
	D3DXVECTOR3 vAngle(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vScale(1.0f, 1.0f, 1.0f);

	CPlayer* pPlayer = new CPlayer(vPos, vAngle, vScale);
	g_Task.InsertObj(pPlayer,ObjName::ObjPlayer);

	//カメラ
	CMainCamera* pCamera = new CMainCamera();
	g_Task.InsertObj(pCamera, ObjName::ObjMainCamera);


	//2DOBJ
	C2DPolygon* p2D = new C2DPolygon();
	g_Task.InsertObj(p2D, ObjName::ObjDraw2D);

	//町
	//CStreet* pStreet = new CStreet(vPos, vAngle, vScale);
	//g_Task.InsertObj(pStreet, ObjName::ObjStreet);

	//RayTest
	vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//vScale = D3DXVECTOR3(5.0f, 5.0f, 5.0f);
	CRayTest* pRayTest = new CRayTest(vPos, vAngle, vScale);
	g_Task.InsertObj(pRayTest, ObjName::ObjRayTest);
	
	//シェーダーにカメラセット
	g_Shader.SetCamera(pCamera);
	g_Draw.SetCamera(pCamera);
	
}

//更新
void CSceneMain::Update()
{
	g_Task.Update();
	g_Audio.Update();
}

//描画
void CSceneMain::Draw()
{
	g_Task.Draw();
	g_Obb.Draw();
}

void CSceneMain::Release()
{
	g_Task.Release();
	g_Audio.DeleteMusic();
	g_Loader.Release();
}

//音楽
void CSceneMain::LoadAudio()
{

}

//テクスチャ
void CSceneMain::LoadTexture()
{
	g_Draw.LoadTexture(0, "Texture/Test.png",512,512);
}

//メッシュ
void CSceneMain::LoadMesh()
{
//	g_Loader.LoadMesh(ModelName::ModelPlayer, "Model/Player/Player.obj");

	g_Loader.LoadMesh(ModelName::ModelPlayer, "Model/RayTest/Triangle_Cube.obj");
//	g_Loader.LoadMesh(ModelName::ModelStreet, "Model/Street/Street.obj");
	
	//RayTest

	//三角ポリゴン
	//g_Loader.LoadMesh(ModelName::ModelRayTest, "Model/RayTest/Triangle_Cube.obj");

	//四角ポリゴン
	g_Loader.LoadMesh(ModelName::ModelRayTest, "Model/RayTest/Square_Cube.obj");	
}