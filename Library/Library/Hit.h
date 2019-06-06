#pragma once

#include "CObj.h"
#include "Main.h"
#include <vector>
using namespace std;

//���f�[�^
struct SphereData
{
	D3DXVECTOR3 m_vPos;	//�ʒu
	float m_fRadius;	//���a
};

//�����蔻��
struct CollisionData
{
	CObj3DBase* m_pObj;	//�I�u�W�F�N�g�̃|�C���^
	MY_MESH* m_pMesh;	//���b�V���|�C���^
	int m_Id;			//�I�u�W�F�N�gID

	SphereData m_SphereData;	//���f�[�^
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
	
	vector<CollisionData* > m_Collision;	//�����蔻��f�[�^
};

extern CHit g_Hit;