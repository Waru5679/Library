
#include "Obb.h"
#include "Math.h"
#include "CObj.h"

COBB g_Obb;

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

//������
void COBB::Init()
{
	//HitBox�`��p�I�u�W�F�N�g
	g_Loader.LoadObj("HitBox/HitBox.obj", &m_Mesh);

	//�`��F
	SetColor(1.0f, 1.0f, 1.0f, 0.5f, m_fColor);
}

//OBB�o�^
void COBB::Insert(OBB_Data* obb)
{
	m_Data.push_back(obb);
}

//OBB�̃Z�b�g
OBB_Data COBB::SetOBB(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale, D3DXVECTOR3 Min, D3DXVECTOR3 Max, int ObjName, CObj* p)
{
	OBB_Data obb;

	D3DXMATRIX matScale = MakeMatScale(Scale);
	D3DXMATRIX matRot = MakeMatRot(Angle);
	D3DXMATRIX matTrans = MakeMatTrans(Pos);
	obb.matWorld = matScale * matRot*matTrans;

	//�X�P�[������ő�E�ŏ������߂�
	D3DXVec3TransformCoord(&obb.vMax, &Max, &matScale);
	D3DXVec3TransformCoord(&obb.vMin, &Min, &matScale);

	//���S�_
	D3DXVECTOR3 vTempPos = (obb.vMin + obb.vMax) * 0.5f + Pos;
	obb.m_Pos = vTempPos;

	//��]
	obb.m_Angle = Angle;

	//�X�P�[��
	obb.m_Scale = Scale;

	//�����擾 
	obb.m_Length[0] = fabsf(obb.vMax.x - obb.vMin.x) /** 0.5f*/;
	obb.m_Length[1] = fabsf(obb.vMax.y - obb.vMin.y) /** 0.5f*/;
	obb.m_Length[2] = fabsf(obb.vMax.z - obb.vMin.z) /** 0.5f*/;

	//���S����̒���
	obb.m_Direct[0] = D3DXVECTOR3(matRot._11, matRot._12, matRot._13);
	obb.m_Direct[1] = D3DXVECTOR3(matRot._21, matRot._22, matRot._23);
	obb.m_Direct[2] = D3DXVECTOR3(matRot._31, matRot._32, matRot._33);

	obb.Name=ObjName;
	obb.p=p;

	//���_
	obb.VerPos[0] = D3DXVECTOR3(vTempPos.x + obb.vMin.x, vTempPos.y + obb.vMax.y, vTempPos.z + obb.vMin.z);
	obb.VerPos[1] = D3DXVECTOR3(vTempPos.x + obb.vMax.x, vTempPos.y + obb.vMax.y, vTempPos.z + obb.vMin.z);
	obb.VerPos[2] = D3DXVECTOR3(vTempPos.x + obb.vMax.x, vTempPos.y + obb.vMin.y, vTempPos.z + obb.vMin.z);
	obb.VerPos[3] = D3DXVECTOR3(vTempPos.x + obb.vMin.x, vTempPos.y + obb.vMin.y, vTempPos.z + obb.vMin.z);

	obb.VerPos[4] = D3DXVECTOR3(vTempPos.x + obb.vMin.x, vTempPos.y + obb.vMax.y, vTempPos.z + obb.vMax.z);
	obb.VerPos[5] = D3DXVECTOR3(vTempPos.x + obb.vMax.x, vTempPos.y + obb.vMax.y, vTempPos.z + obb.vMax.z);
	obb.VerPos[6] = D3DXVECTOR3(vTempPos.x + obb.vMax.x, vTempPos.y + obb.vMin.y, vTempPos.z + obb.vMax.z);
	obb.VerPos[7] = D3DXVECTOR3(vTempPos.x + obb.vMin.x, vTempPos.y + obb.vMin.y, vTempPos.z + obb.vMax.z);

	//�@��
	for (int i = 0; i < 6; i++)
	{
		D3DXVec3TransformCoord(&obb.Norm[i], &g_Norm[i], &matRot);
	}

	//���b�V�����
	obb.pMesh = dynamic_cast<CObj3D*>(p)->GetMesh();

	return obb;

}

//�����蔻��(�I�u�W�F�N�g���w��)
vector<HIT_DATA> COBB::ObjNameHit(OBB_Data* Obb,int Name)
{
	vector<HIT_DATA> vecData;
	HIT_DATA data;
	bool bHit=false;
	for (unsigned int i = 0; i<m_Data.size(); i++)
	{
		if (m_Data[i] != Obb)
		{
			if (m_Data[i]->Name == Name)
			{
				bHit = Collision(Obb, m_Data[i]);
				if (bHit == true)
				{
					//�ڍ׏�������

					//����
					D3DXVECTOR3 vDir = Obb->m_Pos - m_Data[i]->m_Pos;
					data.vDir = vDir;
					
					//����
					data.fDis = D3DXVec3Length(&vDir);
				
					//�����̐��K��
					D3DXVec3Normalize(&vDir, &vDir);
					data.vnDir = vDir;	

					//�I�u�W�F�N�g�̃T�C�Y
					data.vMin = m_Data[i]->vMin;
					data.vMax = m_Data[i]->vMax;
					
					//�I�u�W�F�N�g�̃|�C���^
					data.p=m_Data[i]->p;

					//OBB�̃|�C���^
					data.obb = m_Data[i];

					vecData.push_back(data);
				}
			}
		}
	}
	return vecData;
}


//�X�V
void COBB::Update(OBB_Data* obb,D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale, D3DXVECTOR3 Min, D3DXVECTOR3 Max)
{
	D3DXMATRIX matScale = MakeMatScale(Scale);
	D3DXMATRIX matRot = MakeMatRot(Angle);
	D3DXMATRIX matTrans = MakeMatTrans(Pos);
	obb->matWorld = matScale * matRot*matTrans;

	//�ŏ��_�A�ő�_
	D3DXVec3TransformCoord(&obb->vMax, &Max, &matScale);
	D3DXVec3TransformCoord(&obb->vMin, &Min, &matScale);

	//���S�_
	D3DXVECTOR3 vTempPos = (Min + Max) * 0.5f + Pos;
	obb->m_Pos = vTempPos;

	//��]
	obb->m_Angle = Angle;

	//�X�P�[��
	obb->m_Scale = Scale;

	//�����擾
	obb->m_Length[0] = fabsf(obb->vMax.x - obb->vMin.x)/**0.5f*/;
	obb->m_Length[1] =fabsf(obb->vMax.y - obb->vMin.y)/**0.5f*/;
	obb->m_Length[2] =fabsf(obb->vMax.z - obb->vMin.z)/**0.5f*/;

	//���S����̒���
	obb->m_Direct[0] = D3DXVECTOR3(matRot._11, matRot._12, matRot._13);
	obb->m_Direct[1] = D3DXVECTOR3(matRot._21, matRot._22, matRot._23);
	obb->m_Direct[2] = D3DXVECTOR3(matRot._31, matRot._32, matRot._33);
	
	//���_
	obb->VerPos[0] = D3DXVECTOR3(vTempPos.x + obb->vMin.x, vTempPos.y + obb->vMax.y, vTempPos.z + obb->vMin.z);
	obb->VerPos[1] = D3DXVECTOR3(vTempPos.x + obb->vMax.x, vTempPos.y + obb->vMax.y, vTempPos.z + obb->vMin.z);
	obb->VerPos[2] = D3DXVECTOR3(vTempPos.x + obb->vMax.x, vTempPos.y + obb->vMin.y, vTempPos.z + obb->vMin.z);
	obb->VerPos[3] = D3DXVECTOR3(vTempPos.x + obb->vMin.x, vTempPos.y + obb->vMin.y, vTempPos.z + obb->vMin.z);

	obb->VerPos[4] = D3DXVECTOR3(vTempPos.x + obb->vMin.x, vTempPos.y + obb->vMax.y, vTempPos.z + obb->vMax.z);
	obb->VerPos[5] = D3DXVECTOR3(vTempPos.x + obb->vMax.x, vTempPos.y + obb->vMax.y, vTempPos.z + obb->vMax.z);
	obb->VerPos[6] = D3DXVECTOR3(vTempPos.x + obb->vMax.x, vTempPos.y + obb->vMin.y, vTempPos.z + obb->vMax.z);
	obb->VerPos[7] = D3DXVECTOR3(vTempPos.x + obb->vMin.x, vTempPos.y + obb->vMin.y, vTempPos.z + obb->vMax.z);

	//�@��
	for (int i = 0; i < 6; i++)
	{
		D3DXVec3TransformCoord(&obb->Norm[i], &g_Norm[i], &matRot);
	}

}

//OBB�̕`��
void COBB::Draw()
{
	for (unsigned int i = 0; i < m_Data.size(); i++)
	{
		g_Loader.Draw(m_Data[i]->matWorld, &m_Mesh, m_fColor);
	}
}

//�߂荞�ݏC��
D3DXVECTOR3 COBB::NestingCorrection(OBB_Data* StopObb, OBB_Data* MoveObb)
{
	//OBB�̒��_�̗v�f��
	int VerNum[6] = { 0,1,5,4,0,3 };

	D3DXVECTOR3 Point[6];	//���ʏ�̓_
	D3DXVECTOR3 Norm[6];	//�@��

	for (int i = 0; i < 6; i++)
	{
		Norm[i] = StopObb->Norm[i];
		Point[i] = StopObb->VerPos[ VerNum[i] ];
	}

	//�X�P�[��
	D3DXVECTOR3 Scale = MoveObb->m_Scale;

	//���S�_����ʂւ̂̕����x�N�g��
	D3DXVECTOR3 Dir_x = MoveObb->m_Direct[0];
	D3DXVECTOR3 Dir_y = MoveObb->m_Direct[1];
	D3DXVECTOR3 Dir_z = MoveObb->m_Direct[2];

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
		r =  ux+uy+uz;
		
		D3DXVECTOR3 PC = MoveObb->m_Pos - Point[i];
		b = fabsf(D3DXVec3Dot(&PC, &Norm[i]));

		if (r < b)
		{
			//�������ĂȂ�
			a = -1;
		}
		else
		{
			float e = D3DXVec3Dot(&PC, &Norm[i]);
			if ( e<= 0)
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

// �������ɓ��e���ꂽ���������瓊�e���������Z�o
float COBB::LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 =0)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	// ������Sep�͕W��������Ă��邱��
	float r1 = (float)fabs(D3DXVec3Dot(Sep, e1));
	float r2 = (float)fabs(D3DXVec3Dot(Sep, e2));
	float r3 = e3 ? ((float)fabs(D3DXVec3Dot(Sep, e3))) : 0;
	return r1 + r2 + r3;
}

//�Փ˔���
bool COBB::Collision(OBB_Data* obb1, OBB_Data* obb2)
{
	// �e�����x�N�g���̊m��
	// �iN***:�W���������x�N�g���j
	D3DXVECTOR3 NAe1 = obb1->m_Direct[0], Ae1 = NAe1 * obb1->m_Length[0];
	D3DXVECTOR3 NAe2 = obb1->m_Direct[1], Ae2 = NAe2 * obb1->m_Length[1];
	D3DXVECTOR3 NAe3 = obb1->m_Direct[2], Ae3 = NAe3 * obb1->m_Length[2];

	D3DXVECTOR3 NBe1 = obb2->m_Direct[0], Be1 = NBe1 * obb2->m_Length[0];
	D3DXVECTOR3 NBe2 = obb2->m_Direct[1], Be2 = NBe2 * obb2->m_Length[1];
	D3DXVECTOR3 NBe3 = obb2->m_Direct[2], Be3 = NBe3 * obb2->m_Length[2];
	D3DXVECTOR3 Interval = obb1->m_Pos - obb2->m_Pos;
	
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
	L =(float)fabs(D3DXVec3Dot(&Interval, &NBe1));
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
	L =(float) fabs(D3DXVec3Dot(&Interval, &Cross));
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



