#include "Scene.h"
#include "Task.h"

CSceneManager g_Scene;

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

void CSceneManager::Update()
{
	m_pNowScene->Update();
}

void CSceneManager::Draw()
{
	m_pNowScene->Draw();
}

