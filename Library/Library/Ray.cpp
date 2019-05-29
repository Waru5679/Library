#include "Ray.h"
CRay g_Ray;

//�o�^
void CRay::Insert(CObj3D* pObj)
{
	m_Data.push_back(pObj);
}

//Ray�̓����蔻��
bool CRay::RayHit(D3DXVECTOR3* OutPoint, D3DXVECTOR3 Pos, D3DXVECTOR3 RayVec)
{
	MY_MESH* pMesh;

	//Mesh�̐�����
	for (unsigned int MeshNum = 0; MeshNum < m_Data.size(); MeshNum++)
	{
		pMesh = m_Data[MeshNum]->GetMesh();

		//Material�̐�����
		for (unsigned int MaterialNum = 0; MaterialNum < pMesh->Material.size(); MaterialNum++)
		{
			//�ʂ̐�������
			for (unsigned int FaceNum = 0; FaceNum < pMesh->Material[MaterialNum].FaceInfo.size(); FaceNum++)
			{
				//�ʏ��
				FACE_INFO face = pMesh->Material[MaterialNum].FaceInfo[FaceNum];

				//���_��
				int VerNum = face.FaceofVer;
				
				//�O�p�|���S��
				if (VerNum == 3)
				{
					//Ray����
					if (TriangleRay(OutPoint, Pos, RayVec, face.pvPos[0], face.pvPos[1],face.pvPos[2],face.vNorm) == true)
					{
						//�ǂ���
						WallShear(OutPoint, RayVec, face.vNorm);
						return true;
					}
				}
				//�l�p�|���S��
				if (VerNum == 4)
				{
					//���ς̕����������ɂȂ��Ēʂ�Ȃ����ۂ�
					//Ray����
					//if (SquareRay(OutPoint, Pos, RayVec, face.pVer[0].vPos, face.pVer[1].vPos, face.pVer[3].vPos, face.pVer[2].vPos,face.vNorm) == true)
					//{
					//	//�ǂ���
					//	WallShear(&m_vShear, RayVec, m_vNorm);
					//	return true;
					//}

					//if ((face.pVer[0].vNorm == face.pVer[1].vNorm &&
					//	face.pVer[0].vNorm == face.pVer[2].vNorm &&
					//	face.pVer[0].vNorm == face.pVer[3].vNorm) == false)
					//{
					//	int a = 0;

					//}
				}
			}
		}
	}
	return false;
}

//Ray�̔���(�O�p)
bool CRay::TriangleRay(D3DXVECTOR3* OutPoint, D3DXVECTOR3 RayPos, D3DXVECTOR3 RayVec, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC,D3DXVECTOR3 vNorm)
{
	//�ʂ̒��_����Ray�̎n�_�ƏI�_�ւ̃x�N�g��
	D3DXVECTOR3 vStart, vEnd;
	vStart = vA - RayPos;
	vEnd = vA - (RayPos + RayVec);

	//�@���Ǝn�_�I�_�ւ̃x�N�g��
	float dotStart = D3DXVec3Dot(&vStart, &vNorm);
	float dotEnd = D3DXVec3Dot(&vEnd, &vNorm);
		
	
	//Ray�̎n�_�ƏI�_�����ʏ�ɂ���A��_���v�Z�ł��Ȃ��B
	if (dotStart == 0.0f && dotEnd == 0.0f)
	{
		return false;
	}

	//�n�_�ƏI�_�����������ɂ���
	if (dotStart > 0.0f && dotEnd > 0.0f ||
		dotStart < 0.0f && dotEnd < 0.0f)
	{
		return false;
	}
	//��_�����Ƃ߂�
	else
	{
		//���ʂ̕�����
		float a = vNorm.x;
		float b = vNorm.y;
		float c = vNorm.z;
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
		D3DXVECTOR3 vInterPoint;
		vInterPoint.x = x1 + a1 * t;
		vInterPoint.y = y1 + b1 * t;
		vInterPoint.z = z1 + c1 * t;


		//��_�Ƃ��ꂼ��̕ӂ̊O�ς��@���Ɠ����������m��ׂ�

		D3DXVECTOR3 vA_Inter = vA - vInterPoint;	//��_����A�ւ̃x�N�g��
		D3DXVECTOR3 vAB = vA - vB;					//B����A�ւ̃x�N�g��

		//��_��AB�̊O��
		D3DXVECTOR3 vCrossAB;
		D3DXVec3Cross(&vCrossAB, &vA_Inter, &vAB);
		float dotAB = D3DXVec3Dot(&vCrossAB, &vNorm);

		D3DXVECTOR3 vB_Inter = vB - vInterPoint;	//��_����B�ւ̃x�N�g��
		D3DXVECTOR3 vBC = vB - vC;					//C����B�ւ̃x�N�g��

		//��_��AB�̊O��
		D3DXVECTOR3 vCrossBC;
		D3DXVec3Cross(&vCrossBC, &vB_Inter, &vBC);
		float dotBC = D3DXVec3Dot(&vCrossBC, &vNorm);


		D3DXVECTOR3 vC_Inter = vC - vInterPoint;	//��_����C�ւ̃x�N�g��
		D3DXVECTOR3 vCA = vA - vC;					//A����C�ւ̃x�N�g��

		//��_��AB�̊O��
		D3DXVECTOR3 vCrossCA;
		D3DXVec3Cross(&vCrossCA, &vC_Inter, &vCA);
		float dotCA = D3DXVec3Dot(&vCrossCA, &vNorm);

		//�S�Ă̊O�ς��@���Ɠ��������������Ă��鎞��_�͖ʂ̒��ɂ���
		if (dotAB > 0.0f && dotBC > 0.0f && dotCA > 0.0f)
		{
			//��_
			OutPoint = &vInterPoint;
			return true;
		}
	}
	return false;
}


//Ray�̔���(�l�p�|���S��)
bool CRay::SquareRay(D3DXVECTOR3* OutPoint,D3DXVECTOR3 RayPos, D3DXVECTOR3 RayVec, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC, D3DXVECTOR3 vD, D3DXVECTOR3 vNorm)
{
	//���ʏ�̓_A�Ƃ���Ray�̎n�_�ƏI�_�ւ̃x�N�g�������߂�
	D3DXVECTOR3 vA_Start = -(RayPos - vA);
	D3DXVECTOR3 vA_Final = -((RayPos + RayVec) - vA);

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
	float dot1 = D3DXVec3Dot(&vA_Start, &PlaneNorm);
	float dot2 = D3DXVec3Dot(&vA_Final, &PlaneNorm);

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
				float a = PlaneNorm.x;//���ʂ̖@��
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
