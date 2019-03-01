#include "Ray.h"
CRay g_Ray;

//�ʂ��\�����钸�_�̗v�f��
int FaceNum[6][4] =
{
	{0,1,2,3},
	{1,5,6,2},
	{5,4,7,6},
	{4,0,3,7},
	{0,4,5,1},
	{3,7,6,2}
};

bool CRay::RayHit(D3DXVECTOR3* OutPoint,D3DXVECTOR3 Pos, D3DXVECTOR3 RayVec, OBB_Data Obb)
{
	for (int i = 0; i < 6; i++)
	{
		//Ray�̌�������
		if (Ray(OutPoint,Pos, RayVec, Obb.VerPos[FaceNum[i][0]], Obb.VerPos[FaceNum[i][1]], Obb.VerPos[FaceNum[i][2]], Obb.VerPos[FaceNum[i][3]])==true)
		{
			WallShear(&m_vShear, RayVec, m_vNorm);
			return true;
		}
	}

	return false;
}

//Ray�̔���
bool CRay::Ray(D3DXVECTOR3* OutPoint,D3DXVECTOR3 RayPos, D3DXVECTOR3 RayVec, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC, D3DXVECTOR3 vD)
{
	//Ray�̎n�_�ƏI�_
	D3DXVECTOR3 v1, v2;

	//���ʂ��������2�_�ւ̃x�N�g�����~�߂�
	v1 = -(RayPos - vA);
	v2 = -((RayPos + RayVec) - vA);

	//4�ӂ̃x�N�g�������߂�
	D3DXVECTOR3 AB, BC, CD, DA;
	AB = vB - vA;
	BC = vC - vB;
	CD = vD - vC;
	DA = vA - vD;

	//���ʂ̖@��
	D3DXVECTOR3 PlaneNorm;
	D3DXVec3Cross(&PlaneNorm, &AB, &BC);

	//�@���x�N�g����ێ�
	m_vNorm = PlaneNorm;

	//�x�N�g���Ɩ@���̓���
	float dot1 = D3DXVec3Dot(&v1, &PlaneNorm);
	float dot2 = D3DXVec3Dot(&v2, &PlaneNorm);

	//��������
	if (dot1 == 0.0f && dot2 == 0.0f) 
	{
		//���[�����ʏ�ɂ���A��_���v�Z�ł��Ȃ��B
		return false;
	}
	else if ((dot1 >= 0.0f && dot2 <= 0.0f) || (dot1 <= 0.0f && dot2 >= 0.0f)) 
	{
		//���ς̕Е����v���X�ŕЕ����}�C�i�X�Ȃ̂ŁA�������Ă���
		//���ʂ͈͓̔��Ō������Ă��邩���ׂ�

		//Ray�̎n�_����e���_�ւ̃x�N�g��
		D3DXVECTOR3 PA, PB, PC, PD;
		PA = vA - RayPos;
		PB = vB - RayPos;
		PC = vC - RayPos;
		PD = vD - RayPos;

		//�@�������߂�
		D3DXVECTOR3	nPAB, nPBC, nPCD, nPDA;
		D3DXVec3Cross(&nPAB, &PA, &AB);
		D3DXVec3Cross(&nPBC, &PB, &BC);
		D3DXVec3Cross(&nPCD, &PC, &CD);
		D3DXVec3Cross(&nPDA, &PD, &DA);

		//Ray�Ɩ@���̓��ς����߂�
		float dotPAB, dotPBC, dotPCD, dotPDA;
		dotPAB = D3DXVec3Dot(&nPAB, &RayVec);
		dotPBC = D3DXVec3Dot(&nPBC, &RayVec);
		dotPCD = D3DXVec3Dot(&nPCD, &RayVec);
		dotPDA = D3DXVec3Dot(&nPDA, &RayVec);

		//���ς̕��������ׂē����Ȃ瓖�����Ă�
		if (dotPAB >= 0 && dotPBC >= 0 && dotPCD >= 0 && dotPDA >= 0 ||
			dotPAB <= 0 && dotPBC <= 0 && dotPCD <= 0 && dotPDA <= 0)
		{
			if (D3DXVec3Dot(&PlaneNorm, &RayVec) < 0)
			{
				//�|���S���\

				//���ʂ̕�����
				float a = PlaneNorm.x;
				float b = PlaneNorm.y;
				float c = PlaneNorm.z;
				float x = vA.x;
				float y = vA.y;
				float z = vA.z;

				float d = -(a*x + b * y + c * z);

				//Ray�̒������̒萔t�����߂�
				float a1 = RayVec.x;
				float b1 = RayVec.y;
				float c1 = RayVec.z;

				float x1 = RayPos.x;
				float y1 = RayPos.y;
				float z1 = RayPos.z;

				float t = -(a*x1 + b * y1 + c * z1 + d) / (a*a1 + b * b1 + c * c1);

				//�����������_�����߂�
				OutPoint->x = x1 + a1 * t;
				OutPoint->y = y1 + b1 * t;
				OutPoint->z = z1 + c1 * t;
			}
			return true;
		}
		else
		{
			//�������Ă��Ȃ�
			return false;
		}
	}
	else 
	{
		//�������Ă��Ȃ�
		return false;
	}	
}


//�ǂ���
D3DXVECTOR3* CRay::WallShear(D3DXVECTOR3* Out, D3DXVECTOR3 Front, D3DXVECTOR3 Normal)
{
	//�@�����K��
	D3DXVECTOR3 Normal_n;
	D3DXVec3Normalize(&Normal_n, &Normal);
	
	//�ǂ���x�N�g��
	D3DXVECTOR3 Shear = Front - D3DXVec3Dot(&Front, &Normal_n) * Normal_n;
	return D3DXVec3Normalize(Out, &Shear);
}
