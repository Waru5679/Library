#include "LibraryHeader.h"

//�C���X�^���X
CHit* CHit::m_pInstance = nullptr;
 
 //�o�^
 void CHit::Insert(CollisionData* pData)
 {
	 // �o�^
	 m_Collision.push_back(pData);
 }

 //������
 void CHit::Init()
 {
	 //HitBox
	 LOADER->LoadObj("Model/Hit/HitBox.obj", &m_DrawObb);

	 //��
	 LOADER->LoadObj("Model/Hit/Sphere.obj", &m_DrawSphere);

	 //�F
	 m_Color = ColorData(1.0f, 1.0f, 1.0f, 0.5f);

 }

//�����蔻��f�[�^�o�^
CollisionData CHit::CollisionCreate(CObj3DBase* pCobj)
{
	//�����蔻����
	CollisionData coll_data;
	coll_data.m_pObj = pCobj;
	coll_data.m_pMesh = pCobj->GetMesh();
	coll_data.m_Id = pCobj->GetId();
	
	//Obb�f�[�^
	ObbData obb_data;
	SetObbData(pCobj, coll_data.m_pMesh, &obb_data);
	coll_data.m_ObbData = obb_data;

	//�����
	SphereData sphere_data;
	SetSphereData(pCobj,&coll_data.m_ObbData, coll_data.m_pMesh, &sphere_data);
	coll_data.m_SphereData = sphere_data;
		
	return coll_data;
}

//�Փ˔���
bool CHit::Hit()
{
	for (unsigned int i = 0; i < m_Collision.size() - 1; i++)
	{
		for (unsigned int j = i + 1; j < m_Collision.size(); j++)
		{
			//���̏Փ˔���
			if (SphereAndSphre(&m_Collision[i]->m_SphereData, &m_Collision[j]->m_SphereData) == true)
			{
				//OBB�̏Փ˔���
				if (ObbAndObb(&m_Collision[i]->m_ObbData, &m_Collision[j]->m_ObbData) == true)
				{
					return true;
				}
			}
		}
	}

	return false;
}

//�X�V
void CHit::UpData(CollisionData* pData)
{
	//���f�[�^
	SetSphereData(pData->m_pObj,&pData->m_ObbData,pData->m_pMesh, &pData->m_SphereData);

	//OBB�f�[�^
	SetObbData(pData->m_pObj, pData->m_pMesh, &pData->m_ObbData);
}

//�`��
void CHit::Draw()
{
	D3DXMATRIX matWorld;
 
	for (unsigned int i = 0; i < m_Collision.size(); i++)
	{	
		//Obb�`��
		LOADER->Draw(m_Collision[i]->m_ObbData.m_matWorld, &m_DrawObb,&m_Color);

		//���`��
		LOADER->Draw(m_Collision[i]->m_SphereData.m_matWorld,&m_DrawSphere, &m_Color);
	}
}

//���
void CHit::Release()
{
	VectorRelease(m_Collision);

	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}

// �������ɓ��e���ꂽ���������瓊�e���������Z�o
float CHit::LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3 = 0)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	float r1 = (float)fabs(D3DXVec3Dot(Sep, e1));
	float r2 = (float)fabs(D3DXVec3Dot(Sep, e2));
	float r3 = e3 ? ((float)fabs(D3DXVec3Dot(Sep, e3))) : 0;
	return r1 + r2 + r3;
}

//OBB��OBB�̏Փ˔���
bool CHit::ObbAndObb(ObbData* obb1, ObbData* obb2)
{
	// �e�����x�N�g���̊m��
	D3DXVECTOR3 NAe1 = obb1->m_vDirect[0], Ae1 = NAe1 * obb1->m_vLength.x;
	D3DXVECTOR3 NAe2 = obb1->m_vDirect[1], Ae2 = NAe2 * obb1->m_vLength.y;
	D3DXVECTOR3 NAe3 = obb1->m_vDirect[2], Ae3 = NAe3 * obb1->m_vLength.z;

	D3DXVECTOR3 NBe1 = obb2->m_vDirect[0], Be1 = NBe1 * obb2->m_vLength.x;
	D3DXVECTOR3 NBe2 = obb2->m_vDirect[1], Be2 = NBe2 * obb2->m_vLength.y;
	D3DXVECTOR3 NBe3 = obb2->m_vDirect[2], Be3 = NBe3 * obb2->m_vLength.z;
	D3DXVECTOR3 Interval = obb1->m_vCenterPos - obb2->m_vCenterPos;

	// ������ : Ae1
	FLOAT rA = D3DXVec3Length(&Ae1);
	FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	FLOAT L = (float)fabs(D3DXVec3Dot(&Interval, &NAe1));
	if (L > rA + rB)
		return false; // �Փ˂��Ă��Ȃ�

	// ������ : Ae2
	rA = D3DXVec3Length(&Ae2);
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &NAe2));
	if (L > rA + rB)
		return false;

	// ������ : Ae3
	rA = D3DXVec3Length(&Ae3);
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &NAe3));
	if (L > rA + rB)
		return false;

	// ������ : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be1);
	L = (float)fabs(D3DXVec3Dot(&Interval, &NBe1));
	if (L > rA + rB)
		return false;

	// ������ : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be2);
	L = (float)fabs(D3DXVec3Dot(&Interval, &NBe2));
	if (L > rA + rB)
		return false;

	// ������ : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &NBe3));
	if (L > rA + rB)
		return false;

	// ������ : C11
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C12
	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C13
	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C21
	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C22
	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C23
	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C31
	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C32
	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C33
	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
	return true;
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

//�����Z�b�g
void CHit::SetSphereData(CObj3DBase* pCobj,ObbData* pObb, MY_MESH* pMesh,SphereData* pSphere)
{
	//���S�_����ʂ̒���
	float x, y, z;
	x = pObb->m_vLength.x;
	y = pObb->m_vLength.y;
	z = pObb->m_vLength.z;

	//�~�̔��a
	float xy, xz, yz;
	xy = sqrtf(x * x + y * y);
	xz = sqrtf(x * x + z * z);
	yz = sqrtf(y * y + z * z);

	//���̔��a
	float xy_z, xz_y, yz_x;
	xy_z = sqrtf(xy * xy + z * z);
	xz_y = sqrtf(xz * xz + y * y);
	yz_x = sqrtf(yz * yz + x * x);

	//���a
	float radius = MostLongComponent(xy_z, xz_y, yz_x);

	//�`��p�Ƀ��[���h�}�g���b�N�X�����
	D3DXVECTOR3 vPos = pCobj->GetPos();
	D3DXVECTOR3 vAngle = pCobj->GetAngle();
	D3DXVECTOR3 vScale = D3DXVECTOR3(radius,radius,radius);
	D3DXMATRIX	matWorld = MakeMatWorld(vPos, vAngle, vScale);

	//���f�[�^���i�[
	pSphere->m_fRadius = radius;
	pSphere->m_vPos = vPos;
	pSphere->m_matWorld = matWorld;
}

//OBB�ɐ��l�Z�b�g
void CHit::SetObbData(CObj3DBase* pCobj, MY_MESH* pMesh, ObbData* pObb)
{
	D3DXVECTOR3 vPos, vAngle, vScale;
	vPos	= pCobj->GetPos();
	vAngle	= pCobj->GetAngle();
	vScale	= pCobj->GetScale();
	
	//�s��
	D3DXMATRIX matRot = MakeMatRot(vAngle);
	D3DXMATRIX matScale = MakeMatScale(vScale);

	//�X�P�[������ő�E�ŏ������߂�
	D3DXVECTOR3 vMin, vMax;
	D3DXVec3TransformCoord(&vMax, &pMesh->vMax, &matScale);
	D3DXVec3TransformCoord(&vMin, &pMesh->vMin, &matScale);

	//���S�_
	D3DXVECTOR3 vCenterPos = (vMin + vMax) / 2.0f + vPos;
	pObb->m_vCenterPos = vCenterPos;

	//���S�_����e���ւ̒��� 
	pObb->m_vLength = MakeAbsVector3(vMax - vMin) / 2.0f;

	//���S�_����e���ւ̒���
	pObb->m_vDirect[0] = D3DXVECTOR3(matRot._11, matRot._12, matRot._13);
	pObb->m_vDirect[1] = D3DXVECTOR3(matRot._21, matRot._22, matRot._23);
	pObb->m_vDirect[2] = D3DXVECTOR3(matRot._31, matRot._32, matRot._33);

	//�`��p�Ƀ��[���h�}�g���b�N�X���
	pObb->m_matWorld = MakeMatWorld(vCenterPos, vAngle, vScale);
}

//�߂荞�ݏC��
D3DXVECTOR3 CHit::Fixation(D3DXVECTOR3 Pos, D3DXVECTOR3 Norm, ObbData* pObb)
{
	//�@�����K��
	D3DXVECTOR3 Norm_n;
	D3DXVec3Normalize(&Norm_n, &Norm);

	//Obb�̒��S�_
	D3DXVECTOR3 Center = pObb->m_vCenterPos;

	//�ʂ��璆�S�_�ւ̃x�N�g��
	D3DXVECTOR3 FaceFromCenter = Center - Pos;

	//�e���̕����x�N�g��
	D3DXVECTOR3 Dir_x = pObb->m_vDirect[0];
	D3DXVECTOR3 Dir_y = pObb->m_vDirect[1];
	D3DXVECTOR3 Dir_z = pObb->m_vDirect[2];

	//�e���̃x�N�g������
	float Length_x = pObb->m_vLength.x;
	float Length_y = pObb->m_vLength.y;
	float Length_z = pObb->m_vLength.z;

	//�e���̃x�N�g���Ɩʂ̖@���̓���
	float dot_x = D3DXVec3Dot(&(Dir_x * Length_x), &Norm_n);
	float dot_y = D3DXVec3Dot(&(Dir_y * Length_y), &Norm_n);
	float dot_z = D3DXVec3Dot(&(Dir_z * Length_z), &Norm_n);
	
	//���S�ƒ��_�̒���
	float r = fabsf(dot_x) + fabsf(dot_y) + fabsf(dot_z);

	//�ʂƒ��S�̒���
	float s = fabsf(D3DXVec3Dot(&FaceFromCenter, &Norm_n));

	//�ʂ��璆�S�_�ւ̃x�N�g���Ɩ@���̓���
	float dot = D3DXVec3Dot(&FaceFromCenter, &Norm_n);

	//�߂�����
	float fix_length;

	//���S���ʂ̖@���Ɠ��������ɂ���
	if (dot > 0.0f)
	{
		fix_length = r - s;
	}
	//���S���ʂ̖@���Ɣ��΂̕����ɂ���
	else
	{
		fix_length = r + s;
	}

	//�ʂ̖@���̕����֖߂�
	D3DXVECTOR3 vOut = Norm * fix_length;

	return vOut;
}