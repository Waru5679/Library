#include "../Library/LibraryHeader.h"
#include "GameHeader.h"

#include "Test.h"
//������
void CSceneMain::Init()
{
	//�f�ޓǂݍ���
	LoadTexture();
	LoadAudio();
	LoadMesh();
	
	//�J����
	CMainCamera* pCamera = new CMainCamera();
	g_Task.InsertObj(pCamera, ObjName::ObjMainCamera);

	//�v���C���[
	D3DXVECTOR3 vPos(0.0f, 0.0f, -20.0f);
	D3DXVECTOR3 vAngle(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vScale(1.0f, 1.0f, 1.0f);

	CPlayer* pPlayer=new CPlayer(vPos, vAngle, vScale);
	g_Task.InsertObj(pPlayer,ObjName::ObjPlayer);

	//2DOBJ
	C2DPolygon* p2D=new C2DPolygon();
	g_Task.InsertObj(p2D, ObjName::ObjDraw2D);

	//�G
	vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	CEnemy* pEnemy=new CEnemy(vPos, vAngle, vScale);
	g_Task.InsertObj(pEnemy, ObjName::ObjEnemy);

	//�e�X�g

	vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vScale = D3DXVECTOR3(2.0f, 2.0f, 2.0f);

	CTest* pTest = new CTest(vPos, vAngle, vScale);
	g_Task.InsertObj(pTest, ObjName::ObjTest);

	//�V�F�[�_�[�ɃJ�����Z�b�g
	SHADER->SetCamera(dynamic_cast<CCameraBase*>(pCamera));
	SKIN_SHADER->SetCamera(dynamic_cast<CCameraBase*>(pCamera));
	DRAW->SetCamera(dynamic_cast<CCameraBase*>(pCamera));

	AUDIO->StartMusic(0);
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

//�J��
void CSceneMain::Release()
{
	g_Task.Release();
	AUDIO->DeleteMusic();
	LOADER->Release();
	RAY->Release();	
	TEXTURE->Release();
	MODEL->Release();
}

//���y
void CSceneMain::LoadAudio()
{
	AUDIO->Load(0, "Audio/King.wav", true);
}

//�e�N�X�`��
void CSceneMain::LoadTexture()
{
	TEXTURE->Load(0, "Texture/Test.png",512,512);
}

//���b�V��
void CSceneMain::LoadMesh()
{
	//�v���C���[
	MODEL->LoadModel(ModelName::ModelPlayer, "Model/Player/Player.obj");

	//��
	MODEL->LoadModel(ModelName::ModelSphere, "Model/Sphere/Sphere.obj");
	
	//�Ə�
	MODEL->LoadModel(ModelName::ModelAiming, "Model/Aiming/Aiming.obj");

	//�I
	MODEL->LoadModel(ModelName::ModelTarget, "Model/Target/Target.obj");

}