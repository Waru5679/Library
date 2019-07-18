#include "Task.h"
#include "Release.h"

CTask g_Task;

//�I�u�W�F�N�g�o�^
void CTask::InsertObj(CObjBase* pObj, int Id)
{
	m_Obj.push_back(pObj);
	pObj->SetId(Id);
	pObj->SetDelete(false);
	pObj->Init();
}

//�X�V
void CTask::Update()
{
	//OBJ�X�V
	for (auto itr = m_Obj.begin();itr!=m_Obj.end(); )
	{
		(*itr)->Update();

		//�폜
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

//�`��
void CTask::Draw()
{
	//OBJ�`��
	for (auto itr = m_Obj.begin(); itr != m_Obj.end(); itr++)
	{
		(*itr)->Draw();
	}
}

//�I�u�W�F�N�g�擾
CObjBase* CTask::GetObj(int Id)
{
	for(auto itr = m_Obj.begin(); itr != m_Obj.end(); itr++)
	{
		if ((*itr)->GetId() == Id)
			return (*itr);
	}
	return nullptr;
}

//�������̊J��
void CTask::Release()
{
	m_Obj.clear();
}