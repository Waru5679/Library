#include "Task.h"
#include "Shader.h"
#include "Obb.h"
#include "Ray.h"
CTask g_Task;

//�V�[���o�^
void CTask::InsertScene(CScene* Scene, int Id)
{
	m_Scene.insert(m_Scene.begin() + Id, Scene);
	Scene->id = Id;
}

//�e�N�X�`���o�^
void CTask::Insert(ID3D10ShaderResourceView* pTex, int Id)
{
	m_Tex.insert(m_Tex.begin() + Id, pTex);
}

//3DOBJ�o�^
void CTask::Insert3DObj(CObj3D* Obj, int Id)
{
	m_3DObj.push_back(Obj);
	Obj->m_id = Id;
	Obj->m_fDelete = false;
	Obj->Init();
}

//2DOBJ�o�^
void CTask::Insert2DObj(CObj2D* Obj, int Id)
{
	m_2DObj.push_back(Obj);
	Obj->m_id = Id;
	Obj->m_fDelete = false;
	Obj->Init();
}

//�J�����o�^
void CTask::InsertCamera(Camera* pCamera, int Id)
{
	m_Camera.insert( m_Camera.begin()+ Id, pCamera);
	pCamera->Init();
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

	//3DOBJ�X�V
	for (unsigned int i = 0; i < m_3DObj.size(); i++)
	{
		m_3DObj[i]->Update();
	}

	//2DOBJ�X�V
	for (unsigned int i = 0; i < m_2DObj.size(); i++)
	{
		m_2DObj[i]->Update();
	}

	//�J�����X�V
	for (unsigned int i = 0; i < m_Camera.size(); i++)
	{
		m_Camera[i]->Update();
	}
}

//�f�[�^�폜
void CTask::ObjDelete()
{
	//�폜����v�f����ۑ�����
	vector <int> DeleteNum;
	
	//3DOBJ---
	//�폜����v�f��T��
	for (unsigned int i = 0; i < m_3DObj.size(); i++)
	{
		if (m_3DObj[i]->m_fDelete == true)
		{
			DeleteNum.push_back(i);
		}
	}
	
	int count = 0;

	//OBJ�폜
	for (unsigned int i = 0; i < DeleteNum.size(); i++)
	{
		//����
		m_3DObj.erase(m_3DObj.begin() + DeleteNum[i] - count);
		count++;
	}

	//�v�f����j��
	DeleteNum.erase(DeleteNum.begin(), DeleteNum.end());	

	//2DOBJ---
	//�폜����v�f��T��
	for (unsigned int i = 0; i < m_2DObj.size(); i++)
	{
		if (m_2DObj[i]->m_fDelete == true)
		{
			DeleteNum.push_back(i);
		}
	}

	//OBJ�폜
	for (unsigned int i = 0; i < DeleteNum.size(); i++)
	{
		//����
		m_2DObj.erase(m_2DObj.begin() + DeleteNum[i] - count);
		count++;
	}

	//�v�f����j��
	DeleteNum.erase(DeleteNum.begin(), DeleteNum.end());
}

//�`��
void CTask::Draw()
{
	//3DOBJ�`��
	for (unsigned int i = 0; i < m_3DObj.size(); i++)
	{
		m_3DObj[i]->Draw();
	}

	//2DOBJ�`��
	for (unsigned int i = 0; i < m_2DObj.size(); i++)
	{
		m_2DObj[i]->Draw();
	}
}

//3D�I�u�W�F�N�g�擾
CObj3D* CTask::Get3DObj(int Id)
{
	for (unsigned int i = 0; i < m_3DObj.size(); i++)
	{
		if (m_3DObj[i]->m_id == Id)
			return m_3DObj[i];
	}
	return NULL;
}

//2D�I�u�W�F�N�g�擾
CObj2D* CTask::Get2DObj(int Id)
{
	for (unsigned int i = 0; i < m_2DObj.size(); i++)
	{
		if (m_2DObj[i]->m_id == Id)
			return m_2DObj[i];
	}
	return NULL;
}

//�V�[���擾
CScene* CTask::GetScene(int Id)
{
	for (unsigned int i = 0; i < m_Scene.size(); i++)
	{
		if (m_Scene[i]->id == Id)
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

//�J�����擾
Camera* CTask::GetCamera(int Id)
{
	return m_Camera[Id];
}

//�������̊J��
void CTask::Release()
{
	m_3DObj.erase(m_3DObj.begin(), m_3DObj.end());
	m_2DObj.erase(m_2DObj.begin(), m_2DObj.end());
	m_Mesh.erase(m_Mesh.begin(), m_Mesh.end());
	m_Tex.erase(m_Tex.begin(), m_Tex.end());
	m_Camera.erase(m_Camera.begin(), m_Camera.end());
}


