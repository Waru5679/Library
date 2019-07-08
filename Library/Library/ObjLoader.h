#pragma once
#include "Struct.h"
#include <vector>
#include "Singleton.h"
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