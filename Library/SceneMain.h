#pragma once

#include "Scene.h"
class CSceneMain :public CScene
{
public:
	CSceneMain() {};
	~CSceneMain() {};

	void Init();
	void Update();
	void Draw();
	void Release();

private:
	void LoadAudio();		//音楽
	void LoadTexture();		//テクスチャ
	void LoadMesh();		//メッシュ
};