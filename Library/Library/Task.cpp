#include "Task.h"

CTask g_Task;

//�e�N�X�`���o�^
void CTask::Insert(ID3D10ShaderResourceView* pTex, int Id,int Width,int Height)
{
	MY_TEXTURE tex;
	tex.m_pTex = pTex;
	tex.m_Id = Id;
	tex.m_Width = Width;
	tex.m_Height = Height;

	m_Tex.push_back(tex);
}

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
	//OBJ�폜
	ObjDelete();

	//OBJ�X�V
	for (unsigned int i = 0; i < m_Obj.size(); i++)
	{
		m_Obj[i]->Update();
	}
}

//�f�[�^�폜
void CTask::ObjDelete()
{
	//�폜����v�f����ۑ�����
	vector <int> DeleteNum;
	
	//�폜����v�f��T��
	for (unsigned int i = 0; i < m_Obj.size(); i++)
	{
		if (m_Obj[i]->GetDelete() == true)
		{
			DeleteNum.push_back(i);
		}
	}
	
	//�폜�J�E���^�[
	int count = 0;

	//OBJ�폜
	for (unsigned int i = 0; i < DeleteNum.size(); i++)
	{
		//����
		m_Obj.erase(m_Obj.begin() + DeleteNum[i] - count);
		count++;
	}

	//�v�f����j��
	DeleteNum.erase(DeleteNum.begin(), DeleteNum.end());	
}

//�`��
void CTask::Draw()
{
	//OBJ�`��
	for (unsigned int i = 0; i < m_Obj.size(); i++)
	{
		m_Obj[i]->Draw();
	}
}


//�e�N�X�`�����擾
MY_TEXTURE* CTask::GetTex(int Id)
{
	for (unsigned int i = 0; i < m_Tex.size(); i++)
	{
		if (m_Tex[i].m_Id == Id)
			return &m_Tex[i];
	}
	return nullptr;
}

//�I�u�W�F�N�g�擾
CObjBase* CTask::GetObj(int Id)
{
	for (unsigned int i = 0; i < m_Obj.size(); i++)
	{
		if (m_Obj[i]->GetId() == Id)
			return m_Obj[i];
	}
	return nullptr;
}

//�������̊J��
void CTask::Release()
{
	VectorRelease(m_Tex);
	VectorRelease(m_Obj);
}


