#include "Hit.h"
#include "Math.h"

 CHit g_Hit;
 
//�����蔻��f�[�^�o�^
CollisionData CHit::CollisionCreate(CObj3DBase* pCobj)
{
	//�����蔻����
	CollisionData coll_data;
	coll_data.m_pObj = pCobj;
	coll_data.m_pMesh = pCobj->GetMesh();
	coll_data.m_Id = pCobj->GetId();
	
	//���a���߂�
	D3DXVECTOR3 vMax, vMin;
	vMax = coll_data.m_pMesh->vMax;
	vMin = coll_data.m_pMesh->vMin;

	//��Ԓ����������甼�a��ݒ�
	float diameter = MostLongComponent(vMax - vMin);	//���a
	float radius = fabsf(diameter) / 2.0f;				//���a

	//���f�[�^���i�[
	coll_data.m_SphereData.m_fRadius = radius;
	coll_data.m_SphereData.m_vPos = pCobj->GetPos();
	
	return coll_data;
}

void CHit::Insert(CollisionData* pData)
{
	// �o�^
	m_Collision.push_back(pData);
}

//�X�V
void CHit::UpData(CollisionData* pData,D3DXVECTOR3 vPos)
{
	pData->m_SphereData.m_vPos = vPos;
}

//���Փ˔���
bool CHit::SphereHit()
{
	return SphereAndSphre( &m_Collision[0]->m_SphereData, &m_Collision[1]->m_SphereData);
}

//���Ƌ��̏Փ˔���
bool CHit::SphereAndSphre(SphereData* data1, SphereData* data2)
{
	//2�_�Ԃ̋���
	float PosLength;	
	PosLength = D3DXVec3Length(&(data1->m_vPos - data2->m_vPos));
	
	//�Q�̔��a�̘a
	float RadiusLength;
	RadiusLength = data1->m_fRadius + data2->m_fRadius;

	//2�_�Ԃ̋��������a�̘a��菬������Γ������Ă���
	if (PosLength <= RadiusLength)
	{
		return true;
	}

	return false;
}
