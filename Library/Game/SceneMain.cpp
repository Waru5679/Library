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

//Debug
#include "RayTest.h"
#include "Sphere.h"

//������
void CSceneMain::Init()
{
	//�f�ޓǂݍ���
	LoadTexture();
	LoadAudio();
	LoadMesh();
	
	//�v���C���[
	D3DXVECTOR3 vPos(0.0f, 0.0f, -10.0f);
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
/*	CStreet* pStreet = new CStreet(vPos, vAngle, vScale);
	g_Task.InsertObj(pStreet, ObjName::ObjStreet);
*/

	vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//��
	CSphere* pSphere = new CSphere(vPos, vAngle, vScale);
	g_Task.InsertObj(pSphere, ObjName::ObjSphere);

	//RayTest
	//CRayTest* pRayTest = new CRayTest(vPos, vAngle, vScale);
	//g_Task.InsertObj(pRayTest, ObjName::ObjRayTest);
	//

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
	//g_Loader.LoadMesh(ModelName::ModelStreet, "Model/Street/Street.obj");

	//��
	g_Loader.LoadMesh(ModelName::ModelSphere, "Model/Sphere/Sphere.obj");

	//RayTest
	//�O�p�|���S��
	g_Loader.LoadMesh(ModelName::ModelRayTest, "Model/RayTest/Triangle_Cube.obj");

	//�l�p�|���S��
	//g_Loader.LoadMesh(ModelName::ModelRayTest, "Model/RayTest/Square_Cube.obj");	
}