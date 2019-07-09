#pragma once

#include "CObj.h"
#include "Singleton.h"
#include <vector>
using namespace std;

//���f�[�^
struct SphereData
{
	D3DXVECTOR3 m_vPos;		//�ʒu
	float m_fRadius;		//���a

	D3DXMATRIX m_matWorld;	//�`��p
};

//Obb�f�[�^
struct ObbData
{
	D3DXVECTOR3 m_vDirect[3];	//�e���̖ʂ̕���
	D3DXVECTOR3 m_vLength;		//���S�_����e���ւ̒���
	D3DXVECTOR3 m_vCenterPos;	//���S�_�̈ʒu

	D3DXMATRIX	m_matWorld;		//�`��p
};

//�����蔻��
struct CollisionData
{
	CObj3DBase* m_pObj;	//�I�u�W�F�N�g�̃|�C���^
	MY_MESH* m_pMesh;	//���b�V���|�C���^
	int m_Id;			//�I�u�W�F�N�gID

	SphereData	m_SphereData;	//���f�[�^
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
	void UpData(CollisionData* pData);	//�X�V
	void Draw();						//�`��
	void Release();						//���

	//�����蔻��
	bool Hit();

	//�����蔻��f�[�^�o�^
	CollisionData CollisionCreate(CObj3DBase* pCobj);
	void Insert(CollisionData* pData);

	//�߂荞�ݏC��
	D3DXVECTOR3 Fixation(D3DXVECTOR3 Pos, D3DXVECTOR3 Norm, ObbData* pObb);
private:
	//���Ƌ��̃q�b�g����
	bool SphereAndSphre(SphereData* data1, SphereData* data2);

	//OBB�ɃZ�b�g
	void SetObbData(CObj3DBase* pCobj, MY_MESH* pMesh, ObbData* pObb);

	//�����Z�b�g
	void SetSphereData(CObj3DBase* pCobj,ObbData* pObb, MY_MESH* pMesh,SphereData* pSphere);
	
	//�Փ˔���
	bool ObbAndObb(ObbData* obb1, ObbData* obb2);

	// �������ɓ��e���ꂽ���������瓊�e���������Z�o
	float LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3);

	vector<CollisionData* > m_Collision;	//�����蔻��f�[�^

	MY_MESH m_DrawObb;		//�`��pBox
	MY_MESH m_DrawSphere;	//�`��pSphere
	ColorData m_Color;		//�`��F
};