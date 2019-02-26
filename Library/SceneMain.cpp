#include "SceneMain.h"
#include "Task.h"
#include "MainCamera.h"
#include "ObjLoader.h"
#include "Route.h"
#include "Audio.h"
#include "Polygon.h"
#include "Shader.h"

//������
void CSceneMain::Init()
{
	//���y������
	g_Audio.Init();

	//�f�ޓǂݍ���
	LoadTexture();
	LoadAudio();
	LoadMesh();
	
	//�v���C���[
	D3DXVECTOR3 vPos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vAngle(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vScale(1.0f, 1.0f, 1.0f);

	CPlayer* player = new CPlayer(vPos, vAngle, vScale);
	g_Task.Insert3DObj(player,ObjName::Player);



	//�J����
	CMainCamera* camera = new CMainCamera();
	g_Task.InsertCamera(camera, 0);
	
	//�V�F�[�_�[�ɃJ�����Z�b�g
	g_Shader.SetCamera(camera);
	
	D3DXVECTOR3 Pos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Angle(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Scale(1.0f, 1.0f, 1.0f);
		
}

//�X�V
void CSceneMain::Update()
{
	g_Task.Update();
}

//�`��
void CSceneMain::Draw()
{
	g_Task.Draw();
}

void CSceneMain::Release()
{
	g_Task.Release();
	g_Audio.Delete();
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