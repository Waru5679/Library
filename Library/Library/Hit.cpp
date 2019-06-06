#include "Hit.h"
#include "Math.h"

 CHit g_Hit;

//�쐬
SphereData CHit::Create(D3DXVECTOR3 vPos, float fRadius,D3DXVECTOR3 vScale)
{
	SphereData data;
	data.m_vPos = vPos;
	data.m_fRadius=fRadius;
	
	return data;
}

//�o�^
void CHit::Insert(SphereData* pData)
{
	m_data.push_back(pData);
}

//�X�V
void CHit::UpData(SphereData* pData,D3DXVECTOR3 vPos)
{
	pData->m_vPos = vPos;
}

//���Փ˔���
bool CHit::SphereHit()
{
	return SphereAndSphre(m_data[0], m_data[1]);
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
