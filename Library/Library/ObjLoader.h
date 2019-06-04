#pragma once

#include "Struct.h"

//OBJ�f�[�^�ǂݍ��݃N���X
class CObjLoader
{
public:

	//���
	void Release();

	//Mesh�擾
	MY_MESH* GetMesh(int Id);

	//Mesh�̓ǂݍ���
	void LoadMesh(int Id,const char* Name);

	//���b�V���`��
	void Draw( D3DMATRIX matWorld, MY_MESH* pMesh, ColorData* pColor);

	//OBJ�̓ǂݍ���
	HRESULT LoadObj(const char* FileName, MY_MESH* pMesh);
private:
	//�}�e���A���̓ǂݍ���
	HRESULT LoadMaterial(char* FileName, MY_MESH* pMesh);

	//�ő�ƍŏ��̃`�F�b�N
	void MinAndMax(D3DXVECTOR3 Pos,MY_MESH* pMesh);

	//���b�V��
	vector<MY_MESH>m_Mesh;
};

extern CObjLoader g_Loader;