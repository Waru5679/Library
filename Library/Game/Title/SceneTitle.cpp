#include "../../Library/LibraryHeader.h"
#include "../GameHeader.h"

//初期化
void CSceneTitle::Init()
{
	//素材読み込み
	LoadTexture();
	LoadAudio();
	LoadMesh();

	//カメラ
	CTitleCamera* pCamera = new CTitleCamera();
	g_Task.InsertObj(pCamera, ObjName::ObjTitleCamera);

	//タイトルオブジェクト
	CTitleObj* pObj = new CTitleObj();
	g_Task.InsertObj(pObj, ObjName::ObjTitle);

	//シェーダーにカメラセット
	SHADER->SetCamera(dynamic_cast<CCameraBase*>(pCamera));
	SKIN_SHADER->SetCamera(dynamic_cast<CCameraBase*>(pCamera));
	DRAW->SetCamera(dynamic_cast<CCameraBase*>(pCamera));

}

//更新
void CSceneTitle::Update()
{
	g_Task.Update();
	AUDIO->Update();
}

//描画
void CSceneTitle::Draw()
{
	g_Task.Draw();
}

//開放
void CSceneTitle::Release()
{
	g_Task.Release();
	AUDIO->DeleteMusic();
}

//音楽
void CSceneTitle::LoadAudio()
{
}

//テクスチャ
void CSceneTitle::LoadTexture()
{
	TEXTURE->Load(TexName::TexTitle, "Texture/Title.png", 640, 480);
}

//メッシュ
void CSceneTitle::LoadMesh()
{

}