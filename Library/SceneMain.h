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
	void LoadAudio();		//���y
	void LoadTexture();		//�e�N�X�`��
	void LoadMesh();		//���b�V��
};