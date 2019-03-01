#include "SceneMain.h"
#include "Task.h"
#include "MainCamera.h"
#include "ObjLoader.h"
#include "Audio.h"
#include "Polygon.h"
#include "Shader.h"

//������
void CSceneMain::Init()
{
	//�f�ޓǂݍ���
	LoadTexture();
	LoadAudio();
	LoadMesh();
	
	//�v���C���[
	D3DXVECTOR3 vPos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vAngle(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vScale(1.0f, 1.0f, 1.0f);

	CPlayer* pPlayer = new CPlayer(vPos, vAngle, vScale);
	g_Task.InsertObj(pPlayer,Obj_Player);

	//�J����
	CMainCamera* pCamera = new CMainCamera();
	g_Task.InsertObj(pCamera,Obj_MainCamera);
	
	//�V�F�[�_�[�ɃJ�����Z�b�g
	g_Shader.SetCamera(pCamera);
	g_Draw.SetCamera(pCamera);
	
}

//�X�V
void CSceneMain::Update()
{
	g_Task.Update();
	g_Audio.Update();
}

//�`��
void CSceneMain::Draw()
{
	g_Task.Draw();
}

void CSceneMain::Release()
{
	g_Task.Release();
	g_Audio.DeleteMusic();
}

//���y
void CSceneMain::LoadAudio()
{

}

//�e�N�X�`��
void CSceneMain::LoadTexture()
{

}

//���b�V��
void CSceneMain::LoadMesh()
{
	g_Loader.LoadMesh(0, "Player.obj");
}