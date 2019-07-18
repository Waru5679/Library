#include "Task.h"
#include "Release.h"

CTask g_Task;

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
	for (auto itr = m_Obj.begin();itr!=m_Obj.end(); )
	{
		(*itr)->Update();

		//削除
		if ((*itr)->GetDelete() == true)
		{
			m_Obj.erase(itr++);
		}
		else
		{
			itr++;
		}
	}
}

//描画
void CTask::Draw()
{
	//OBJ描画
	for (auto itr = m_Obj.begin(); itr != m_Obj.end(); itr++)
	{
		(*itr)->Draw();
	}
}

//オブジェクト取得
CObjBase* CTask::GetObj(int Id)
{
	for(auto itr = m_Obj.begin(); itr != m_Obj.end(); itr++)
	{
		if ((*itr)->GetId() == Id)
			return (*itr);
	}
	return nullptr;
}

//メモリの開放
void CTask::Release()
{
	m_Obj.clear();
}