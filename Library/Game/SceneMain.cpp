//���C�u�����t�@�C��
#include "../Library/Task.h"
#include "../Library/ObjLoader.h"
#include "../Library/Audio.h"
#include "../Library/Polygon.h"
#include "../Library/Shader.h"
#include "../Library/Ray.h" 

//�Q�[���t�@�C��
#include "2DPolygon.h"
#include "Street.h"
#include "MainCamera.h"
#include "SceneMain.h"
#include "Enemy.h"

//Debug
#include "RayTest.h"
#include "Sphere.h"

#include <memory>
using namespace std;

//������
void CSceneMain::Init()
{
	//�f�ޓǂݍ���
	LoadTexture();
	LoadAudio();
	LoadMesh();
	
	//�v���C���[
	D3DXVECTOR3 vPos(0.0f, 0.0f, -20.0f);
	D3DXVECTOR3 vAngle(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vScale(1.0f, 1.0f, 1.0f);

	CPlayer* pPlayer=new CPlayer(vPos, vAngle, vScale);
	g_Task.InsertObj(pPlayer,ObjName::ObjPlayer);

	//�J����
	CMainCamera* pCamera = new CMainCamera();
	g_Task.InsertObj(pCamera, ObjName::ObjMainCamera);

	//2DOBJ
	C2DPolygon* p2D=new C2DPolygon();
	g_Task.InsertObj(p2D, ObjName::ObjDraw2D);

	//�G
	vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	CEnemy* pEnemy=new CEnemy(vPos, vAngle, vScale);
	g_Task.InsertObj(pEnemy, ObjName::ObjEnemy);

	//�V�F�[�_�[�ɃJ�����Z�b�g
	SHADER->SetCamera(dynamic_cast<CameraBase*>(pCamera));
	g_Draw.SetCamera(dynamic_cast<CameraBase*>(pCamera));
}

//�X�V
void CSceneMain::Update()
{
	g_Task.Update();
	AUDIO->Update();
}

//�`��
void CSceneMain::Draw()
{
	g_Task.Draw();
}

void CSceneMain::Release()
{
	g_Task.Release();
	AUDIO->DeleteMusic();
	g_Loader.Release();
	g_Ray.Release();	



}

//���y
void CSceneMain::LoadAudio()
{

}

//�e�N�X�`��
void CSceneMain::LoadTexture()
{
	g_Draw.LoadTexture(0, "Texture/Test.png",512,512);
}

//���b�V��
void CSceneMain::LoadMesh()
{
	//�v���C���[
	g_Loader.LoadMesh(ModelName::ModelPlayer, "Model/Player/Player.obj");

	//��
	g_Loader.LoadMesh(ModelName::ModelSphere, "Model/Sphere/Sphere.obj");

}