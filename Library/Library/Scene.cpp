#include "Scene.h"
#include "Task.h"

CSceneManager g_Scene;

//登録
void CSceneManager::Insert(CSceneBase* pScene, int Id)
{
	MY_SCENE scene;
	scene.m_pScene = pScene;
	scene.m_Id = Id;

	m_Scene.push_back(scene);
}

//シーン取得
CSceneBase* CSceneManager::GetScene(int Id)
{
	for (int i = 0; i < m_Scene.size(); i++)
	{
		if (m_Scene[i].m_Id == Id)
			return m_Scene[i].m_pScene;
	}
	return nullptr;
}

//シーンセット
void CSceneManager::SetScene(int n)
{
	//現在のシーンの開放
	if(m_pNowScene!=nullptr)
		m_pNowScene->Release();

	//次のシーンの呼び出し
	m_pNowScene = GetScene(n);

	//次のシーンの初期化
	m_pNowScene->Init();
}

//シーンの更新
void CSceneManager::Update()
{
	m_pNowScene->Update();
}

//シーンの描画
void CSceneManager::Draw()
{
	m_pNowScene->Draw();
}

//解放
void CSceneManager::Release()
{
	delete m_pNowScene;
	VectorRelease(m_Scene);	
}

