#pragma once

#include "ModelManager.h"
#include "Singleton.h"
#include <list>
using namespace std;

//OBJ�f�[�^�ǂݍ��݃N���X
class CObjLoader:public CSingleton<CObjLoader>
{
private:
	//�V���O���g��
	friend CSingleton<CObjLoader>;
	CObjLoader() {};	//�R���X�g���N�^
	~CObjLoader() {};	//�f�X�g���N�^
public:
	//���
	void Release();

	//���b�V���`��
	void Draw( D3DMATRIX matWorld, CModelData* pMesh, ColorData* pColor);

	//OBJ�̓ǂݍ���
	HRESULT LoadObj(const char* FileName, CModelData* pMesh);
private:
	//�}�e���A���̓ǂݍ���
	HRESULT LoadMaterial(char* FileName, CModelData* pMesh);

	//�ő�ƍŏ��̃`�F�b�N
	void MinAndMax(D3DXVECTOR3 Pos, CModelData* pMesh);
};