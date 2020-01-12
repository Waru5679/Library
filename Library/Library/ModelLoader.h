#pragma once
#include "Main.h"
#include "Singleton.h"
#include "Struct.h"
#include <stdio.h>

//���_�\����
struct VERTEX
{
	VERTEX()
	{
		/*m_pBoneId = nullptr;
		m_pfWeight = nullptr;*/
	}
	~VERTEX()
	{
		////�{�[��ID
		//if (m_pBoneId != nullptr)
		//{
		//	delete[] m_pBoneId;
		//	m_pBoneId = nullptr;
		//}
		////�E�F�C�g
		//if (m_pfWeight != nullptr)
		//{
		//	delete[] m_pfWeight;
		//	m_pfWeight = nullptr;
		//}
	}

	D3DXVECTOR3 vPos;	//�ʒu
	D3DXVECTOR3 vNorm;	//�@��
	D3DXVECTOR2 vUv;	//UV

	//int		WeightNum;	//�E�F�C�g��
	//int*	m_pBoneId;	//�e�����󂯂�{�[��ID
	//float*	m_pfWeight;	//�E�F�C�g
};

//�|���S��
struct FACE
{
	FACE()
	{
		m_pIndexBuffer = nullptr;
	}
	~FACE()
	{
		//�C���f�b�N�X�o�b�t�@
		if (m_pIndexBuffer != nullptr)
		{
			m_pIndexBuffer->Release();
		}
	}
	int m_VerId[3];						//�\�����钸�_ID
	int m_UseMateId;					//�g�p����}�e���A��ID
	ID3D10Buffer* m_pIndexBuffer;		//�C���f�b�N�X�o�b�t�@�[
};

//�}�e���A��
struct MATERIAL
{
	char m_Name[100];//�}�e���A����
	
	D3DXVECTOR4 m_vColor;	//�ʂ̐F
	D3DXVECTOR4 m_vDiffuse;	//�f�B�t���[�Y
	D3DXVECTOR4 m_vSpecular;//�X�y�L�����[
	D3DXVECTOR4 m_vAmbient;	//�A���r�G���g
	float		m_fSpePower;//�X�y�L�����[�p���[
	int			m_TexId;	//�e�N�X�`��ID
};

//���f���f�[�^
struct MODEL_DATA
{
	MODEL_DATA()
	{
		m_pVertex = nullptr;
		m_pFace = nullptr;
		m_pMaterial = nullptr;
		m_pVertexBuffer = nullptr;
		m_ppTexture = nullptr;
	}
	~MODEL_DATA()
	{
		//���_
		if (m_pVertex != nullptr)
		{
			delete[] m_pVertex;
			m_pVertex = nullptr;
		}
		//��
		if (m_pFace != nullptr)
		{
			delete[] m_pFace;
			m_pFace = nullptr;
		}
		//�}�e���A��
		if (m_pMaterial != nullptr)
		{
			delete[] m_pMaterial;
			m_pMaterial = nullptr;
		}
		//�o�[�e�b�N�X�o�b�t�@
		if (m_pVertexBuffer!= nullptr)
		{
			m_pVertexBuffer->Release();
		}
		//�e�N�X�`��
		for (int i = 0; i < m_TexNum; i++)
		{
			m_ppTexture[i]->Release();
		}
	}

	char						m_Name[20];		//���f����
	int							m_VerNum;		//���_��
	int							m_FaceNum;		//��(�|���S��)��
	int							m_TexNum;		//�e�N�X�`����
	int							m_MateNum;		//�}�e���A���̐�
	VERTEX*						m_pVertex;		//���_���̃��X�g
	FACE*						m_pFace;		//�ʃ��X�g
	MATERIAL*					m_pMaterial;	//�}�e���A�����X�g
	ID3D10Buffer*				m_pVertexBuffer;//�o�[�e�b�N�X�o�b�t�@
	ID3D10ShaderResourceView**	m_ppTexture;	//�e�N�X�`��
};

//���f���ǂݍ���
class CModelLoader:public CSingleton<CModelLoader>
{
private:
	//�V���O���g��
	friend CSingleton<CModelLoader>;
	CModelLoader() {};	//�R���X�g���N�^
	~CModelLoader() {};	//�f�X�g���N�^
public:
	void Release();	//���

	//���f���ǂ݂���
	bool Load(const char* FileName, MODEL_DATA* pModel);
	//���f���`��
	void Draw(D3DMATRIX matWorld, MODEL_DATA* pModel, CColorData* pColor);

private:
	//���_�ǂݍ���
	bool VertexLoad(FILE* fp, MODEL_DATA* pModel);
	//�ʓǂݍ���
	bool FaceLoad(FILE* fp, MODEL_DATA* pModel);
	//�e�N�X�`���ǂݍ���
	bool TexLoad(FILE* fp, MODEL_DATA* pModel);
	//�}�e���A���ǂݍ���
	bool MateLoad(FILE* fp, MODEL_DATA* pModel);
};