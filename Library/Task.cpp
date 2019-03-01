#include "Task.h"

CTask g_Task;

//シーン登録
void CTask::InsertScene(CScene* Scene, int Id)
{
	m_Scene.insert(m_Scene.begin() + Id, Scene);
	Scene->m_id = Id;
}

//テクスチャ登録
void CTask::Insert(ID3D10ShaderResourceView* pTex, int Id)
{
	m_Tex.insert(m_Tex.begin() + Id, pTex);
}

//オブジェクト登録
void CTask::InsertObj(CObj* pObj, int Id)
{
	m_Obj.push_back(pObj);
	pObj->m_id = Id;
	pObj->m_fDelete = false;
	pObj->Init();
}

//メッシュ登録
void CTask::Insert(MY_MESH Mesh, int Id)
{
	m_Mesh.insert(m_Mesh.begin() + Id, Mesh);
}

//更新
void CTask::Update()
{
	//OBJ削除
	ObjDelete();

	//OBJ更新
	for (unsigned int i = 0; i < m_Obj.size(); i++)
	{
		m_Obj[i]->Update();
	}
}

//データ削除
void CTask::ObjDelete()
{
	//削除する要素数を保存する
	vector <int> DeleteNum;
	
	//削除する要素を探す
	for (unsigned int i = 0; i < m_Obj.size(); i++)
	{
		if (m_Obj[i]->m_fDelete == true)
		{
			DeleteNum.push_back(i);
		}
	}
	
	//削除カウンター
	int count = 0;

	//OBJ削除
	for (unsigned int i = 0; i < DeleteNum.size(); i++)
	{
		//消去
		m_Obj.erase(m_Obj.begin() + DeleteNum[i] - count);
		count++;
	}

	//要素数を破棄
	DeleteNum.erase(DeleteNum.begin(), DeleteNum.end());	
}

//描画
void CTask::Draw()
{
	//OBJ描画
	for (unsigned int i = 0; i < m_Obj.size(); i++)
	{
		m_Obj[i]->Draw();
	}
}

//シーン取得
CScene* CTask::GetScene(int Id)
{
	for (unsigned int i = 0; i < m_Scene.size(); i++)
	{
		if (m_Scene[i]->m_id == Id)
			return m_Scene[i];
	}
	return NULL;
}

//テクスチャを取得
ID3D10ShaderResourceView* CTask::GetTex(int Id)
{
	return m_Tex[Id];
}

//メッシュ取得
MY_MESH CTask::GetMesh(int Id)
{
	return m_Mesh[Id];
}

//オブジェクト取得
CObj* CTask::GetObj(int Id)
{
	for (unsigned int i = 0; i < m_Obj.size(); i++)
	{
		if (m_Obj[i]->m_id == Id)
			return m_Obj[i];
	}
	return nullptr;
}

//メモリの開放
void CTask::Release()
{
	m_Mesh.erase(m_Mesh.begin(), m_Mesh.end());
	m_Tex.erase(m_Tex.begin(), m_Tex.end());
	m_Obj.erase(m_Obj.begin(), m_Obj.end());
}


