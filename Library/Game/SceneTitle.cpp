#include "../Library/LibraryHeader.h"
#include "GameHeader.h"

//������
void CSceneTitle::Init()
{
	//�f�ޓǂݍ���
	LoadTexture();
	LoadAudio();
	LoadMesh();

	//�J����
	CTitleCamera* pCamera = new CTitleCamera();
	g_Task.InsertObj(pCamera, ObjName::ObjTitleCamera);

	//�^�C�g���I�u�W�F�N�g
	CTitleObj* pObj = new CTitleObj();
	g_Task.InsertObj(pObj, ObjName::ObjTitle);

	//�V�F�[�_�[�ɃJ�����Z�b�g
	SHADER->SetCamera(dynamic_cast<CCameraBase*>(pCamera));
	SKIN_SHADER->SetCamera(dynamic_cast<CCameraBase*>(pCamera));
	DRAW->SetCamera(dynamic_cast<CCameraBase*>(pCamera));

}

//�X�V
void CSceneTitle::Update()
{
	g_Task.Update();
	AUDIO->Update();
}

//�`��
void CSceneTitle::Draw()
{
	g_Task.Draw();
}

//�J��
void CSceneTitle::Release()
{
	g_Task.Release();
	AUDIO->DeleteMusic();
	LOADER->Release();
	RAY->Release();
	TEXTURE->Release();
	MODEL->Release();
}

//���y
void CSceneTitle::LoadAudio()
{
	AUDIO->Load(0, "Audio/King.wav", true);
}

//�e�N�X�`��
void CSceneTitle::LoadTexture()
{
	TEXTURE->Load(0, "Texture/Test.png", 512, 512);
}

//���b�V��
void CSceneTitle::LoadMesh()
{

}