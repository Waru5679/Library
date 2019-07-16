#pragma once

#include <d3dx10.h>
#include <vector>
#include "Release.h"
#include "Singleton.h"
using namespace std;


//���_�\����
class CVertex
{
public:
	//�J��
	void Release() {};

	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vTex;
};

//�ʃf�[�^
class CFaceData
{
public:
	//�J��
	void Release()
	{
		for (unsigned int i = 0; i < m_Vertex.size(); i++)
		{
			m_Vertex[i].Release();
		}
		VectorRelease(m_Vertex);
	}

	vector<CVertex> m_Vertex;	//���_���
};

//�}�e���A���f�[�^
class CMaterialData
{
public:
	//�J��
	void Release()
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

	D3DXVECTOR3 Ka;//�A���r�G���g(����)
	D3DXVECTOR3 Kd;//�f�B�t���[�Y(�g�U��)
	D3DXVECTOR3 Ks;//�X�y�L�����[(���ʔ��ˌ��j

	//�ʏ��
	vector<CFaceData> m_Face;

	//�e�N�X�`��
	ID3D10ShaderResourceView* m_pTexture;

	//�o�b�t�@�[
	vector<ID3D10Buffer*> m_pIndex;
	vector<ID3D10Buffer*> m_pVertex;
};

//���f���f�[�^�N���X
class CModelData
{
public:
	//�J��
	void Release()
	{
		for (unsigned int i = 0; i < m_Material.size(); i++)
			m_Material[i].Release();
		
		VectorRelease(m_Material);
	}

	vector<CMaterialData> m_Material;	//�}�e���A��

	D3DXVECTOR3 m_vMin;	//���_�̍ŏ����W
	D3DXVECTOR3 m_vMax;	//���_�̍ő���W

	int m_Id;//�Ǘ��p��ID
};

//�I�u�W�F�N�g�Ǘ��N���X
class CModelManager :public CSingleton<CModelManager>
{
private:
	//�V���O���g��
	friend CSingleton<CModelManager>;
	CModelManager() {};	//�R���X�g���N�^
	~CModelManager() {};//�f�X�g���N�^
public:
	//���f���擾
	CModelData* GetModelData(int id);
private:
	list<CModelData> m_List;//���f�����X�g
};