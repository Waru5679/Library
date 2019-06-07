#include "Hit.h"
#include "Math.h"

 CHit g_Hit;
 
//�����蔻��f�[�^�o�^
CollisionData CHit::CollisionCreate(CObj3DBase* pCobj)
{
	//HitBox�`��p�I�u�W�F�N�g
	g_Loader.LoadObj("Model/HitBox/HitBox.obj", &m_DrawObb);

	//�����蔻����
	CollisionData coll_data;
	coll_data.m_pObj = pCobj;
	coll_data.m_pMesh = pCobj->GetMesh();
	coll_data.m_Id = pCobj->GetId();
	
	//�����
	coll_data.m_SphereData = SetSphereData(pCobj, coll_data.m_pMesh);
	
	//Obb�f�[�^
	coll_data.m_ObbData = SetObbData(pCobj,coll_data.m_pMesh);
	
	return coll_data;
}

//�����Z�b�g
SphereData CHit::SetSphereData(CObj3DBase* pCobj, MY_MESH* pMesh)
{
	SphereData sphere_data;

	//���a���߂�
	D3DXVECTOR3 vMax, vMin;
	vMax = pMesh->vMax;
	vMin = pMesh->vMin;

	//�X�P�[��
	D3DXVECTOR3 vScale = pCobj->GetScale();
	
	//��ԑ傫���{�����Ƃ�
	float scale=MostLongComponent(vScale);
	
	//debug�p���̃T�C�Y�ύX
	pCobj->SetScale(scale);

	//��Ԓ����������甼�a��ݒ�
	float diameter = MostLongComponent(vMax - vMin)* scale;	//���a
	float radius = fabsf(diameter) / 2.0f;					//���a

	//���f�[�^���i�[
	sphere_data.m_fRadius = radius;
	sphere_data.m_vPos = pCobj->GetPos();

	return sphere_data;
}

//OBB�ɐ��l�Z�b�g
ObbData CHit::SetObbData(CObj3DBase* pCobj,MY_MESH* pMesh)
{
	ObbData obb_data;
	
	//OBB�f�[�^
	obb_data.m_vPos = pCobj->GetPos();
	obb_data.m_vAngle = pCobj->GetAngle();
	obb_data.m_vScale = pCobj->GetScale();
	
	//�s��
	D3DXMATRIX matRot = MakeMatRot(obb_data.m_vAngle);
	D3DXMATRIX matScale=MakeMatScale(obb_data.m_vScale);
	
	//�X�P�[������ő�E�ŏ������߂�
	D3DXVECTOR3 vMin, vMax;
	D3DXVec3TransformCoord(&vMax, &pMesh->vMax, &matScale);
	D3DXVec3TransformCoord(&vMin, &pMesh->vMin, &matScale);

	//���S�_
	D3DXVECTOR3 vCenterPos = (vMin + vMax) / 2.0f + obb_data.m_vPos;
	obb_data.m_vCenterPos = vCenterPos;

	//���S�_����e���ւ̒��� 
	obb_data.m_vLength = MakeAbsVector3(vMax - vMin) / 2.0f;

	//���S�_����e���ւ̒���
	obb_data.m_vDirect[0] = D3DXVECTOR3(matRot._11, matRot._12, matRot._13);
	obb_data.m_vDirect[1] = D3DXVECTOR3(matRot._21, matRot._22, matRot._23);
	obb_data.m_vDirect[2] = D3DXVECTOR3(matRot._31, matRot._32, matRot._33);

	//���_�̈ʒu
	obb_data.m_vVertexPos[0] = D3DXVECTOR3(vCenterPos.x + vMin.x, vCenterPos.y + vMax.y, vCenterPos.z + vMin.z);
	obb_data.m_vVertexPos[1] = D3DXVECTOR3(vCenterPos.x + vMax.x, vCenterPos.y + vMax.y, vCenterPos.z + vMin.z);
	obb_data.m_vVertexPos[2] = D3DXVECTOR3(vCenterPos.x + vMax.x, vCenterPos.y + vMin.y, vCenterPos.z + vMin.z);
	obb_data.m_vVertexPos[3] = D3DXVECTOR3(vCenterPos.x + vMin.x, vCenterPos.y + vMin.y, vCenterPos.z + vMin.z);
	obb_data.m_vVertexPos[4] = D3DXVECTOR3(vCenterPos.x + vMin.x, vCenterPos.y + vMax.y, vCenterPos.z + vMax.z);
	obb_data.m_vVertexPos[5] = D3DXVECTOR3(vCenterPos.x + vMax.x, vCenterPos.y + vMax.y, vCenterPos.z + vMax.z);
	obb_data.m_vVertexPos[6] = D3DXVECTOR3(vCenterPos.x + vMax.x, vCenterPos.y + vMin.y, vCenterPos.z + vMax.z);
	obb_data.m_vVertexPos[7] = D3DXVECTOR3(vCenterPos.x + vMin.x, vCenterPos.y + vMin.y, vCenterPos.z + vMax.z);

	return obb_data;
}

//�o�^
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
