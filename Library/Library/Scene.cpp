#include "Scene.h"
#include "Task.h"

CSceneManager g_Scene;

//�V�[���Z�b�g
void CSceneManager::SetScene(int n)
{
	//���݂̃V�[���̊J��
	if(m_pNowScene!=nullptr)
		m_pNowScene->Release();

	//���̃V�[���̌Ăяo��
	m_pNowScene = g_Task.GetScene(n);

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

