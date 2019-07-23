#include "ModelManager.h"
#include "LibraryHeader.h"

//�C���X�^���X
CModelManager* CModelManager::m_pInstance = nullptr;

//���f���ǂݍ���
void CModelManager::LoadModel(int Id, const char* Name)
{
	CModelData Mesh;
	Mesh.m_Id = Id;

	//�ǂݍ���
	LOADER->LoadObj(Name, &Mesh);

	//�o�^
	m_List.push_back(Mesh);
}

//���f���擾
CModelData* CModelManager::GetModelData(int id)
{
	for (auto itr = m_List.begin(); itr != m_List.end(); itr++)
	{
		if ((*itr).m_Id == id)
			return &(*itr);
	}
	return nullptr;
}

//���f���`��
void CModelManager::Draw(D3DMATRIX matWorld, CModelData* pMesh, CColorData* pColor)
{
	//�}�e���A���̐����ɕ`��
	for (unsigned int i = 0; i < pMesh->m_Material.size(); i++)
	{
		int size = pMesh->m_Material[i].m_pVertex.size();

		//�V�F�[�_�[�̃Z�b�g
		SHADER->SetShader(pMesh->m_Material[i].m_pTexture, NULL, pColor, matWorld);

		for (int j = 0; j < size; j++)
		{
			//�|���S���`��
			DRAW->DrawPolygon(pMesh->m_Material[i].m_Face[j].m_Vertex.size(), pMesh->m_Material[i].m_pVertex[j], pMesh->m_Material[i].m_pIndex[j]);
		}
	}
}

//�J��
void CModelManager::Release()
{
	//���f���f�[�^�j��
	for (auto itr = m_List.begin(); itr != m_List.end(); itr++)
	{
		(*itr).Release();
	}
	m_List.clear();

	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}

//�ʃf�[�^���
void CFaceData::Release()
{
	//���_���
	for (unsigned int i = 0; i < m_Vertex.size(); i++)
	{
		m_Vertex[i].Release();
	}
	VectorRelease(m_Vertex);
}

//�}�e���A�����
void CMaterialData::Release()
{
	//�ʏ��
	for (unsigned int i = 0; i < m_Face.size(); i++)
	{
		m_Face[i].Release();
	}
	VectorRelease(m_Face);

	//�C���f�b�N�X�o�b�t�@�[
	for (unsigned int i = 0; i < m_pIndex.size(); i++)
	{
		m_pIndex[i]->Release();
	}
	VectorRelease(m_pIndex);

	//�o�[�e�b�N�X�o�b�t�@�[
	for (unsigned int i = 0; i < m_pVertex.size(); i++)
	{
		m_pVertex[i]->Release();
	}
	VectorRelease(m_pVertex);

	//�e�N�X�`��
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

//���f���f�[�^���
void CModelData::Release()
{
	//�}�e���A��
	for (unsigned int i = 0; i < m_Material.size(); i++)
	{
		m_Material[i].Release();
	}
	VectorRelease(m_Material);
}
