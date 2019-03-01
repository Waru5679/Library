#include "SceneMain.h"
#include "Task.h"
#include "MainCamera.h"
#include "ObjLoader.h"
#include "Audio.h"
#include "Polygon.h"
#include "Shader.h"

//初期化
void CSceneMain::Init()
{
	//素材読み込み
	LoadTexture();
	LoadAudio();
	LoadMesh();
	
	//プレイヤー
	D3DXVECTOR3 vPos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vAngle(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vScale(1.0f, 1.0f, 1.0f);

	CPlayer* pPlayer = new CPlayer(vPos, vAngle, vScale);
	g_Task.InsertObj(pPlayer,Obj_Player);

	//カメラ
	CMainCamera* pCamera = new CMainCamera();
	g_Task.InsertObj(pCamera,Obj_MainCamera);
	
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
}

void CSceneMain::Release()
{
	g_Task.Release();
	g_Audio.DeleteMusic();
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