#include "Task.h"

CTask g_Task;

//テクスチャ登録
void CTask::Insert(ID3D10ShaderResourceView* pTex, int Id,int Width,int Height)
{
	MY_TEXTURE tex;
	tex.m_pTex = pTex;
	tex.m_Id = Id;
	tex.m_Width = Width;
	tex.m_Height = Height;

	m_Tex.push_back(tex);
}

//オブジェクト登録
void CTask::InsertObj(CObjBase* pObj, int Id)
{
	m_Obj.push_back(pObj);
	pObj->SetId(Id);
	pObj->SetDelete(false);
	pObj->Init();
}

//更新
void CTask::Update()
{
	//OBJ更新
	for (unsigned int i = 0; i < m_Obj.size(); i++)
	{
		m_Obj[i]->Update();

		//削除
		if (m_Obj[i]->GetDelete() == true)
		{
			m_Obj.erase(m_Obj.begin()+i);
			i--;
		}
	}
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


//テクスチャを取得
MY_TEXTURE* CTask::GetTex(int Id)
{
	for (unsigned int i = 0; i < m_Tex.size(); i++)
	{
		if (m_Tex[i].m_Id == Id)
			return &m_Tex[i];
	}
	return nullptr;
}

//オブジェクト取得
CObjBase* CTask::GetObj(int Id)
{
	for (unsigned int i = 0; i < m_Obj.size(); i++)
	{
		if (m_Obj[i]->GetId() == Id)
			return m_Obj[i];
	}
	return nullptr;
}

//メモリの開放
void CTask::Release()
{
	VectorRelease(m_Tex);
	VectorRelease(m_Obj);
}


