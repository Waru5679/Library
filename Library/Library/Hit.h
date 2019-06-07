#pragma once

#include "CObj.h"
#include "Main.h"
#include "ObjLoader.h"
#include <vector>
using namespace std;

//���f�[�^
struct SphereData
{
	D3DXVECTOR3 m_vPos;	//�ʒu
	float m_fRadius;	//���a
};

//Obb�f�[�^
struct ObbData
{
	D3DXVECTOR3 m_vPos;		//�ʒu
	D3DXVECTOR3 m_vAngle;	//��]
	D3DXVECTOR3 m_vScale;	//�X�P�[��

	D3DXVECTOR3 m_vDirect[3];	//�e���̖ʂ̕���
	D3DXVECTOR3 m_vLength;		//���S�_����e���ւ̒���
	D3DXVECTOR3 m_vCenterPos;	//���S�_�̈ʒu

	D3DXVECTOR3 m_vVertexPos[8];//���_�̈ʒu

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
class CHit
{
public:
	bool SphereHit();
	void UpData(CollisionData* pData, D3DXVECTOR3 vPos);

	//�����蔻��f�[�^�o�^
	CollisionData CollisionCreate(CObj3DBase* pCobj);
	void Insert(CollisionData* pData);
private:
	//���Ƌ��̃q�b�g����
	bool SphereAndSphre(SphereData* data1, SphereData* data2);

	//OBB�ɃZ�b�g
	ObbData SetObbData(CObj3DBase* pCobj, MY_MESH* pMesh);

	//�����Z�b�g
	SphereData SetSphereData(CObj3DBase* pCobj, MY_MESH* pMesh);
	
	vector<CollisionData* > m_Collision;	//�����蔻��f�[�^

	MY_MESH m_DrawObb;		//�`��pBox
	MY_MESH m_DrawSphere;	//�`��pSphere

};

extern CHit g_Hit;