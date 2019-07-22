#pragma once

#include "CObj.h"
#include "ModelManager.h"
#include "Singleton.h"
#include <vector>
using namespace std;

//���f�[�^
class CSphereData
{
public:
	//�J��
	void Release() {};

	D3DXVECTOR3 m_vPos;		//�ʒu
	float m_fRadius;		//���a

	D3DXMATRIX m_matWorld;	//�`��p
};

//Obb�f�[�^
class ObbData
{
public:
	//�J��
	void Release() {};

	D3DXVECTOR3 m_vDirect[3];	//�e���̖ʂ̕���
	D3DXVECTOR3 m_vLength;		//���S�_����e���ւ̒���
	D3DXVECTOR3 m_vCenterPos;	//���S�_�̈ʒu

	D3DXMATRIX	m_matWorld;		//�`��p
};

//�����蔻��
class CCollisionData
{
public:
	//�J��
	void Release()
	{
		m_SphereData.Release();
		m_ObbData.Release();
		m_pObj=nullptr;
		m_pMesh=nullptr;
	}

	CObj3DBase* m_pObj;	//�I�u�W�F�N�g�̃|�C���^
	CModelData* m_pMesh;//���b�V���|�C���^
	int m_Id;			//�I�u�W�F�N�gID

	CSphereData	m_SphereData;	//���f�[�^
	ObbData		m_ObbData;		//OBB�f�[�^
};

//�����蔻��N���X
class CHit:public CSingleton<CHit>
{
private:
	//�V���O���g��
	friend CSingleton<CHit>;
	CHit() {}	//�R���X�g���N�^
	~CHit() {}	//�f�X�g���N�^
public:
	void Init();						//������
	void UpData(CCollisionData* pData);	//�X�V
	void Draw();						//�`��
	void Release();						//���

	//�����蔻��
	bool Hit();

	//�����蔻��f�[�^�o�^
	CCollisionData CollisionCreate(CObj3DBase* pCobj);
	void Insert(CCollisionData* pData);

	//�߂荞�ݏC��
	D3DXVECTOR3 Fixation(D3DXVECTOR3 Pos, D3DXVECTOR3 Norm, ObbData* pObb);
private:
	//���Ƌ��̃q�b�g����
	bool SphereAndSphre(CSphereData* data1, CSphereData* data2);

	//OBB�ɃZ�b�g
	void SetObbData(CObj3DBase* pCobj, CModelData* pMesh, ObbData* pObb);

	//�����Z�b�g
	void SetSphereData(CObj3DBase* pCobj,ObbData* pObb, CModelData* pMesh,CSphereData* pSphere);
	
	//�Փ˔���
	bool ObbAndObb(ObbData* obb1, ObbData* obb2);

	// �������ɓ��e���ꂽ���������瓊�e���������Z�o
	float LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3);

	vector<CCollisionData*> m_Collision;	//�����蔻��f�[�^

	CModelData m_DrawObb;		//�`��pBox
	CModelData m_DrawSphere;	//�`��pSphere
	CColorData m_Color;		//�`��F
};