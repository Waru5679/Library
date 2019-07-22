#pragma once

#include <d3dx10.h>
#include <vector>
#include "Release.h"
#include "Singleton.h"
#include "Struct.h"

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
	void Release();

	vector<CVertex> m_Vertex;	//���_���
};

//�}�e���A���f�[�^
class CMaterialData
{
public:
	//�J��
	void Release();

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
	void Release();

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
	//���f���ǂݍ���
	void LoadModel(int Id, const char* Name);

	//���f���擾
	CModelData* GetModelData(int id);

	//���f���`��
	void Draw(D3DMATRIX matWorld, CModelData* pMesh, CColorData* pColor);

private:
	list<CModelData> m_List;//���f�����X�g
};