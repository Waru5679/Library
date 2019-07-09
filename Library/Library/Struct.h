#pragma once

#include <d3dx10.h>
#include <vector>
#include "Release.h"

using namespace std;

//RECT�\����
struct RECT_F
{
	//�R���X�g���N�^
	RECT_F() {}
	RECT_F(float Top, float Left, float Width, float Height)
	{
		m_Top = Top;
		m_Left = Left;
		m_Right = Left + Width;
		m_Bottom = Top + Height;
	}

	float m_Top;	//��
	float m_Left;	//��
	float m_Right;	//�E
	float m_Bottom;	//��
};

//�F�f�[�^
struct ColorData
{
	//�R���X�g���N�^
	ColorData() {}
	ColorData(float R, float G, float B, float A)
	{
		m_Red = R;
		m_Green = G;
		m_Blue = B;
		m_Alpha = A;
	}
	
	float m_Red;	//��
	float m_Green;	//��
	float m_Blue;	//��
	float m_Alpha;	//����
};

//���_�\����
struct MY_VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vTex;
};

//�ʏ��
struct FACE_INFO
{
	//�f�X�g���N�^
	~FACE_INFO()
	{
		VectorRelease(Vertex);
	}
	vector<MY_VERTEX> Vertex;	//���_���
};

//�}�e���A���\����
struct MY_MATERIAL
{

	//�f�X�g���N�^
	~MY_MATERIAL()
	{
		VectorRelease(FaceInfo);
		VectorRelease(pIndex);
		VectorRelease(pVertex);

		if (pTexture != nullptr)
		{
			pTexture = nullptr;
		}
	}

	D3DXVECTOR3 Ka;//�A���r�G���g(����)
	D3DXVECTOR3 Kd;//�f�B�t���[�Y(�g�U��)
	D3DXVECTOR3 Ks;//�X�y�L�����[(���ʔ��ˌ��j

	//�ʏ��
	vector<FACE_INFO> FaceInfo;

	//�e�N�X�`��
	ID3D10ShaderResourceView* pTexture;

	//�o�b�t�@�[
	vector<ID3D10Buffer*> pIndex;
	vector<ID3D10Buffer*> pVertex;
};

//�I���W�i�����b�V��
struct MY_MESH
{
	//�f�X�g���N�^
	~MY_MESH()
	{
		VectorRelease(Material);
	}
	vector<MY_MATERIAL> Material;	//�}�e���A��

	D3DXVECTOR3 vMin;	//���_�̍ŏ����W
	D3DXVECTOR3 vMax;	//���_�̍ő���W

	int Id;//�Ǘ��p��ID
};
