#pragma once
#include "Main.h"
#include "ObjLoader.h"

//�I�u�W�F�N�g�@�p���p�N���X(�Q�[���I�u�W�F�N�g�S�ĂɌp��������)
class CObj
{
public:
	virtual void Init() = 0;	//������
	virtual void Update() = 0;	//�X�V
	virtual void Draw() = 0;	//�`��

	int m_id;		//�I�u�W�F�N�gID
	bool m_fDelete;	//�폜�t���O

protected:
};

//2D�I�u�W�F�N�g�@�p���p�N���X
class CObj2D:public CObj
{
protected:
	D3DXVECTOR2 m_ScreenPos;	//�`��ʒu(����)
	D3DXVECTOR2 m_Move;			//�ړ��x�N�g��

};

//3D���f�����g�p������́@�p���p�N���X
class CObj3D :public CObj
{
public :
	MY_MESH* GetMesh() { return m_pMesh; }
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

	MY_MESH* m_pMesh;	//���f���̃|�C���^
};

