#include "SceneMain.h"
#include "Task.h"
#include "MainCamera.h"
#include "ObjLoader.h"
#include "Audio.h"
#include "Polygon.h"
#include "Shader.h"
#include "2DPolygon.h"
#include "Street.h"

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
	g_Task.InsertObj(pPlayer,ObjName::ObjPlayer);

	//�J����
	CMainCamera* pCamera = new CMainCamera();
	g_Task.InsertObj(pCamera, ObjName::ObjMainCamera);


	//2DOBJ
	C2DPolygon* p2D = new C2DPolygon();
	g_Task.InsertObj(p2D, ObjName::ObjDraw2D);

	//��
	vScale = D3DXVECTOR3(5.0f, 5.0f, 5.0f);

	CStreet* pStreet = new CStreet(vPos, vAngle, vScale);
	g_Task.InsertObj(pStreet, ObjName::ObjStreet);

	
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
	g_Loader.Release();
}

//���y
void CSceneMain::LoadAudio()
{

}

//�e�N�X�`��
void CSceneMain::LoadTexture()
{
	g_Draw.LoadTexture(0, "Texture/Test.png");
}

//���b�V��
void CSceneMain::LoadMesh()
{
	g_Loader.LoadMesh(ModelName::ModelPlayer, "Player/Player.obj");
	g_Loader.LoadMesh(ModelName::ModelStreet, "Street/Street.obj");

}