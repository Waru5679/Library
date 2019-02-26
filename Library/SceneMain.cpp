#include "SceneMain.h"
#include "Task.h"
#include "MainCamera.h"
#include "ObjLoader.h"
#include "Route.h"
#include "Audio.h"
#include "Polygon.h"
#include "Shader.h"

//初期化
void CSceneMain::Init()
{
	//音楽初期化
	g_Audio.Init();

	//素材読み込み
	LoadTexture();
	LoadAudio();
	LoadMesh();
	
	//プレイヤー
	D3DXVECTOR3 vPos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vAngle(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vScale(1.0f, 1.0f, 1.0f);

	CPlayer* player = new CPlayer(vPos, vAngle, vScale);
	g_Task.Insert3DObj(player,ObjName::Player);



	//カメラ
	CMainCamera* camera = new CMainCamera();
	g_Task.InsertCamera(camera, 0);
	
	//シェーダーにカメラセット
	g_Shader.SetCamera(camera);
	
	D3DXVECTOR3 Pos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Angle(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Scale(1.0f, 1.0f, 1.0f);
		
}

//更新
void CSceneMain::Update()
{
	g_Task.Update();
}

//描画
void CSceneMain::Draw()
{
	g_Task.Draw();
}

void CSceneMain::Release()
{
	g_Task.Release();
	g_Audio.Delete();
}

//音楽
void CSceneMain::LoadAudio()
{
}

//テクスチャ
void CSceneMain::LoadTexture()
{

}

//メッシュ
void CSceneMain::LoadMesh()
{
	g_Loader.LoadMesh(0, "Player.obj");
}