#include "Task.h"

CTask g_Task;

//�V�[���o�^
void CTask::InsertScene(CScene* Scene, int Id)
{
	m_Scene.insert(m_Scene.begin() + Id, Scene);
	Scene->m_id = Id;
}

//�e�N�X�`���o�^
void CTask::Insert(ID3D10ShaderResourceView* pTex, int Id)
{
	m_Tex.insert(m_Tex.begin() + Id, pTex);
}

//�I�u�W�F�N�g�o�^
void CTask::InsertObj(CObj* pObj, int Id)
{
	m_Obj.push_back(pObj);
	pObj->m_id = Id;
	pObj->m_fDelete = false;
	pObj->Init();
}

//���b�V���o�^
void CTask::Insert(MY_MESH Mesh, int Id)
{
	m_Mesh.insert(m_Mesh.begin() + Id, Mesh);
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
		if (m_Obj[i]->m_fDelete == true)
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

//�V�[���擾
CScene* CTask::GetScene(int Id)
{
	for (unsigned int i = 0; i < m_Scene.size(); i++)
	{
		if (m_Scene[i]->m_id == Id)
			return m_Scene[i];
	}
	return NULL;
}

//�e�N�X�`�����擾
ID3D10ShaderResourceView* CTask::GetTex(int Id)
{
	return m_Tex[Id];
}

//���b�V���擾
MY_MESH CTask::GetMesh(int Id)
{
	return m_Mesh[Id];
}

//�I�u�W�F�N�g�擾
CObj* CTask::GetObj(int Id)
{
	for (unsigned int i = 0; i < m_Obj.size(); i++)
	{
		if (m_Obj[i]->m_id == Id)
			return m_Obj[i];
	}
	return nullptr;
}

//�������̊J��
void CTask::Release()
{
	m_Mesh.erase(m_Mesh.begin(), m_Mesh.end());
	m_Tex.erase(m_Tex.begin(), m_Tex.end());
	m_Obj.erase(m_Obj.begin(), m_Obj.end());
}


