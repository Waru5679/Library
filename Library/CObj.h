#pragma once
#include "Main.h"
#include "ObjLoader.h"


//�I�u�W�F�N�g�@�p���p�N���X
class CObj
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	int m_id;
	bool m_fDelete;

protected:
	//�x�N�g��
	D3DXVECTOR3 m_vPos;		//�ʒu
	D3DXVECTOR3 m_vAngle;	//��]
	D3DXVECTOR3 m_vScale;	//�傫��

	//�s��
	D3DXMATRIX m_matWorld;	//���[���h
	D3DXMATRIX m_matTrans;	//�ړ�
	D3DXMATRIX m_matRot;	//��]
	D3DXMATRIX m_matScale;	//�g��E�k��
};

//2D�I�u�W�F�N�g�@�p���p�N���X
class CObj2D:public CObj
{
public:
protected:
	D3DXVECTOR2 m_ScreenPos;	//�`��ʒu(����)
};

//3D���f�����g�p������́@�p���p�N���X
class CObj3D :public CObj
{
public:
protected:
	MY_MESH m_Mesh;	//���f��

	D3DXVECTOR3 m_vMin;	//�ŏ�
	D3DXVECTOR3 m_vMax; //�ő�
};

