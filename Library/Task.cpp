#include "Task.h"
#include "Shader.h"
#include "Obb.h"
#include "Ray.h"
CTask g_Task;

//シーン登録
void CTask::InsertScene(CScene* Scene, int Id)
{
	m_Scene.insert(m_Scene.begin() + Id, Scene);
	Scene->id = Id;
}

//テクスチャ登録
void CTask::Insert(ID3D10ShaderResourceView* pTex, int Id)
{
	m_Tex.insert(m_Tex.begin() + Id, pTex);
}

//3DOBJ登録
void CTask::Insert3DObj(CObj3D* Obj, int Id)
{
	m_3DObj.push_back(Obj);
	Obj->m_id = Id;
	Obj->m_fDelete = false;
	Obj->Init();
}

//2DOBJ登録
void CTask::Insert2DObj(CObj2D* Obj, int Id)
{
	m_2DObj.push_back(Obj);
	Obj->m_id = Id;
	Obj->m_fDelete = false;
	Obj->Init();
}

//カメラ登録
void CTask::InsertCamera(Camera* pCamera, int Id)
{
	m_Camera.insert( m_Camera.begin()+ Id, pCamera);
	pCamera->Init();
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

	//3DOBJ更新
	for (unsigned int i = 0; i < m_3DObj.size(); i++)
	{
		m_3DObj[i]->Update();
	}

	//2DOBJ更新
	for (unsigned int i = 0; i < m_2DObj.size(); i++)
	{
		m_2DObj[i]->Update();
	}

	//カメラ更新
	for (unsigned int i = 0; i < m_Camera.size(); i++)
	{
		m_Camera[i]->Update();
	}
}

//データ削除
void CTask::ObjDelete()
{
	//削除する要素数を保存する
	vector <int> DeleteNum;
	
	//3DOBJ---
	//削除する要素を探す
	for (unsigned int i = 0; i < m_3DObj.size(); i++)
	{
		if (m_3DObj[i]->m_fDelete == true)
		{
			DeleteNum.push_back(i);
		}
	}
	
	int count = 0;

	//OBJ削除
	for (unsigned int i = 0; i < DeleteNum.size(); i++)
	{
		//消去
		m_3DObj.erase(m_3DObj.begin() + DeleteNum[i] - count);
		count++;
	}

	//要素数を破棄
	DeleteNum.erase(DeleteNum.begin(), DeleteNum.end());	

	//2DOBJ---
	//削除する要素を探す
	for (unsigned int i = 0; i < m_2DObj.size(); i++)
	{
		if (m_2DObj[i]->m_fDelete == true)
		{
			DeleteNum.push_back(i);
		}
	}

	//OBJ削除
	for (unsigned int i = 0; i < DeleteNum.size(); i++)
	{
		//消去
		m_2DObj.erase(m_2DObj.begin() + DeleteNum[i] - count);
		count++;
	}

	//要素数を破棄
	DeleteNum.erase(DeleteNum.begin(), DeleteNum.end());
}

//描画
void CTask::Draw()
{
	//3DOBJ描画
	for (unsigned int i = 0; i < m_3DObj.size(); i++)
	{
		m_3DObj[i]->Draw();
	}

	//2DOBJ描画
	for (unsigned int i = 0; i < m_2DObj.size(); i++)
	{
		m_2DObj[i]->Draw();
	}
}

//3Dオブジェクト取得
CObj3D* CTask::Get3DObj(int Id)
{
	for (unsigned int i = 0; i < m_3DObj.size(); i++)
	{
		if (m_3DObj[i]->m_id == Id)
			return m_3DObj[i];
	}
	return NULL;
}

//2Dオブジェクト取得
CObj2D* CTask::Get2DObj(int Id)
{
	for (unsigned int i = 0; i < m_2DObj.size(); i++)
	{
		if (m_2DObj[i]->m_id == Id)
			return m_2DObj[i];
	}
	return NULL;
}

//シーン取得
CScene* CTask::GetScene(int Id)
{
	for (unsigned int i = 0; i < m_Scene.size(); i++)
	{
		if (m_Scene[i]->id == Id)
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

//カメラ取得
Camera* CTask::GetCamera(int Id)
{
	return m_Camera[Id];
}

//メモリの開放
void CTask::Release()
{
	m_3DObj.erase(m_3DObj.begin(), m_3DObj.end());
	m_2DObj.erase(m_2DObj.begin(), m_2DObj.end());
	m_Mesh.erase(m_Mesh.begin(), m_Mesh.end());
	m_Tex.erase(m_Tex.begin(), m_Tex.end());
	m_Camera.erase(m_Camera.begin(), m_Camera.end());
}


