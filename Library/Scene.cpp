#include "Scene.h"
#include "Task.h"

CSceneManager g_Scene;

void CSceneManager::SetScene(int n)
{
	//現在のシーンの開放
	if(m_pNowScene!=nullptr)
		m_pNowScene->Release();

	//次のシーンの呼び出し
	m_pNowScene = g_Task.GetScene(n);

	//次のシーンの初期化
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

