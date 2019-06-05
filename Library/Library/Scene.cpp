#include "Scene.h"
#include "Task.h"

CSceneManager g_Scene;

//�o�^
void CSceneManager::Insert(CSceneBase* pScene, int Id)
{
	MY_SCENE scene;
	scene.m_pScene = pScene;
	scene.m_Id = Id;

	m_Scene.push_back(scene);
}

//�V�[���擾
CSceneBase* CSceneManager::GetScene(int Id)
{
	for (int i = 0; i < m_Scene.size(); i++)
	{
		if (m_Scene[i].m_Id == Id)
			return m_Scene[i].m_pScene;
	}
	return nullptr;
}

//�V�[���Z�b�g
void CSceneManager::SetScene(int n)
{
	//���݂̃V�[���̊J��
	if(m_pNowScene!=nullptr)
		m_pNowScene->Release();

	//���̃V�[���̌Ăяo��
	m_pNowScene = GetScene(n);

	//���̃V�[���̏�����
	m_pNowScene->Init();
}

//�V�[���̍X�V
void CSceneManager::Update()
{
	m_pNowScene->Update();
}

//�V�[���̕`��
void CSceneManager::Draw()
{
	m_pNowScene->Draw();
}

//���
void CSceneManager::Release()
{
	delete m_pNowScene;
	VectorRelease(m_Scene);	
}

