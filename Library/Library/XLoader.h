#pragma once

#include <stdio.h>
#include <windows.h>
#include <d3dx10.h>

//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")

#include "Struct.h"

//�}�N��
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

//�I���W�i���@�}�e���A���\����
struct MY_MATERIAL
{
	CHAR szName[100];		//���O
	D3DXVECTOR4 Kd;			//�f�B�t���[�Y
	float Kp;				//�X�y�L�����[�̃p���[
	D3DXVECTOR3 Ks;			//�X�y�L�����[
	D3DXVECTOR3 Ke;			//�G�~�b�V�u

	//�e�N�X�`��
	CHAR szTextureName[100];			//�e�N�X�`���[�t�@�C����
	ID3D10ShaderResourceView* pTexture;	//�e�N�X�`���|�C���^
	
	DWORD dwNumFace;//���̃}�e���A���ł���|���S����

	//�R���X�g���N�^
	MY_MATERIAL()
	{
		ZeroMemory(this, sizeof(MY_MATERIAL));
	}
	//�f�X�g���N�^
	~MY_MATERIAL()
	{
		SAFE_RELEASE(pTexture);
	}
};

//�I���W�i�����b�V��
struct MY_MESH
{
	//�J��
	void Release()
	{
		delete[] pMaterial;
		delete[] ppIndexBuffer;

		pVertexBuffer->Release();
	}

	DWORD dwNumVert;
	DWORD dwNumFace;
	ID3D10Buffer* pVertexBuffer;
	ID3D10Buffer** ppIndexBuffer;
	DWORD dwNumMaterial;


	MY_MATERIAL* pMaterial;
};

//���_�\����
struct MY_VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vTex;
};


//X�ǂݍ��݃N���X
class CXLoader
{
public:
	//X�t�@�C����ǂݍ��ފ֐�
	HRESULT LoadMesh(const char* FileName, MY_MESH* pMesh);

	//X�`��
	void Draw(D3DMATRIX matWorld, MY_MESH* pMesh, CColorData* pColor);
};