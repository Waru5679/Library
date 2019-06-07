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
	
	//��Ԓ����������甼�a��ݒ�
	float diameter = MostLongComponent(vMax - vMin)* scale;	//���a
	float radius = fabsf(diameter) / 2.0f;					//���a

	//debug�p���̃T�C�Y�ύX
	pCobj->SetScale(scale);

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
	pData->m_ObbData = SetObbData(pData->m_pObj, pData->m_pMesh);
}

//���Փ˔���
bool CHit::Hit(D3DXVECTOR3* vShear)
{
	if (ObbAndObb(&m_Collision[0]->m_ObbData, &m_Collision[1]->m_ObbData) == true)
	{
		D3DXVECTOR3 vOut;
		vOut=NestingCorrection(&m_Collision[1]->m_ObbData, &m_Collision[0]->m_ObbData);
		vShear->x = vOut.x;
		vShear->y = vOut.y;
		vShear->z = vOut.z;

		return true;
	}
	return false;
	//return SphereAndSphre( &m_Collision[0]->m_SphereData, &m_Collision[1]->m_SphereData);
}

//���Ƌ��̏Փ˔���
bool CHit::SphereAndSphre(SphereData* pData1, SphereData* pData2)
{
	//2�_�Ԃ̋���
	float PosLength;	
	PosLength = D3DXVec3Length(&(pData1->m_vPos - pData2->m_vPos));
	
	//�Q�̔��a�̘a
	float RadiusLength;
	RadiusLength = pData1->m_fRadius + pData2->m_fRadius;

	//2�_�Ԃ̋��������a�̘a��菬������Γ������Ă���
	if (PosLength <= RadiusLength)
	{
		return true;
	}

	return false;
}

//OBB��OBB�̔���
bool CHit::ObbAndObb(ObbData* pData1, ObbData* pData2)
{
	// �e�����x�N�g���̊m��
	// �iN***:�W���������x�N�g���j
	D3DXVECTOR3 NAe1 = pData1->m_vDirect[0], Ae1 = NAe1 * pData1->m_vLength.x;
	D3DXVECTOR3 NAe2 = pData1->m_vDirect[1], Ae2 = NAe2 * pData1->m_vLength.y;
	D3DXVECTOR3 NAe3 = pData1->m_vDirect[2], Ae3 = NAe3 * pData1->m_vLength.z;

	D3DXVECTOR3 NBe1 = pData2->m_vDirect[0], Be1 = NBe1 * pData2->m_vLength.x;
	D3DXVECTOR3 NBe2 = pData2->m_vDirect[1], Be2 = NBe2 * pData2->m_vLength.y;
	D3DXVECTOR3 NBe3 = pData2->m_vDirect[2], Be3 = NBe3 * pData2->m_vLength.z;
	D3DXVECTOR3 Interval = pData1->m_vCenterPos - pData2->m_vCenterPos;

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
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C12
	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C13
	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C21
	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C22
	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C23
	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C31
	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C32
	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C33
	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
	return true;
}

// �������ɓ��e���ꂽ���������瓊�e���������Z�o
float CHit::LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	// ������Sep�͕W��������Ă��邱��
	float r1 = (float)fabs(D3DXVec3Dot(Sep, e1));
	float r2 = (float)fabs(D3DXVec3Dot(Sep, e2));

	float r3;
	if (e3 == NULL)
		r3 = 0;
	else
		r3 = ((float)fabs(D3DXVec3Dot(Sep, e3)));

	return r1 + r2 + r3;
}

//�@��
D3DXVECTOR3 g_Norm[] =
{
	D3DXVECTOR3(0,0,-1),
	D3DXVECTOR3(1,0,0),
	D3DXVECTOR3(0,0,1),
	D3DXVECTOR3(-1,0,0),
	D3DXVECTOR3(0,1,0),
	D3DXVECTOR3(0,-1,0)
};

//OBB�̂߂荞�ݏC��
D3DXVECTOR3 CHit::NestingCorrection(ObbData* StopObb, ObbData* MoveObb)
{
	//OBB�̒��_�̗v�f��
	int VerNum[6] = { 0,1,5,4,0,3 };

	D3DXVECTOR3 Point[6];	//���ʏ�̓_
	D3DXVECTOR3 Norm[6];	//�@��

	//��]�s��
	D3DXMATRIX matRot = MakeMatRot(StopObb->m_vAngle);

	for (int i = 0; i < 6; i++)
	{
		//�@��
		D3DXVec3TransformCoord(&Norm[i], &g_Norm[i], &matRot);
		Point[i] = StopObb->m_vVertexPos[ VerNum[i] ];
	}

	//�X�P�[��
	D3DXVECTOR3 Scale = MoveObb->m_vScale;

	//���S�_����ʂւ̂̕����x�N�g��
	D3DXVECTOR3 Dir_x = MoveObb->m_vDirect[0];
	D3DXVECTOR3 Dir_y = MoveObb->m_vDirect[1];
	D3DXVECTOR3 Dir_z = MoveObb->m_vDirect[2];

	//���S����ʂւ̃x�N�g��
	D3DXVECTOR3 ex, ey, ez;
	ex = Dir_x * Scale.x;
	ey = Dir_y * Scale.y;
	ez = Dir_z * Scale.z;

	float r;	//���S�_����Փ˂��Ă钸�_�ւ̒���
	float b;	//���S�_���畽�ʂւ̒���
	float a;	//�߂荞�݂̒���
	for (int i = 0; i < 6; i++)
	{
		float ux, uy, uz;
		ux = fabsf(D3DXVec3Dot(&ex, &Norm[i]));
		uy = fabsf(D3DXVec3Dot(&ey, &Norm[i]));
		uz = fabsf(D3DXVec3Dot(&ez, &Norm[i]));
		r = ux + uy + uz;

		D3DXVECTOR3 PC = MoveObb->m_vCenterPos - Point[i];
		b = fabsf(D3DXVec3Dot(&PC, &Norm[i]));

		if (r < b)
		{
			//�������ĂȂ�
			a = -1;
		}
		else
		{
			float e = D3DXVec3Dot(&PC, &Norm[i]);
			if (e <= 0)
			{
				//PC��n�̓��ς��}�C�i�X
				a = r + b;
			}
			else
			{
				//PC��n�̓��ς��v���X
				a = r - b;
			}
		}

		//�Փ˂��Ă�Ƃ�
		if (a != -1)
		{
			//�@�����߂荞�݂̒���
			return Norm[i] * a;
		}
	}

	//�Փ˂Ȃ�
	return D3DXVECTOR3(0, 0, 0);
}

