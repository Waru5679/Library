#include "Ray.h"
#include "Math.h"
#include "Release.h"

//�C���X�^���X
CRay* CRay::m_pInstance = nullptr;

//�o�^
void CRay::Insert(CObj3DBase* pObj)
{
	m_Data.push_back(pObj);
}

//Ray��Hit����
bool CRay::RayHit(OutData* pOut, CObj3DBase* pOrigin, D3DXVECTOR3 vDir)
{
	return OriginSet(pOut, pOrigin, vDir);
}

//Ray�̔��˓_�����߂�
bool CRay::OriginSet(OutData* pOut,CObj3DBase* pOrigin,D3DXVECTOR3 vDir)
{
	//Ray���΂���
	MY_MESH* pOrigineMesh = pOrigin->GetMesh();
	D3DXVECTOR3 vOrigin;
	D3DXMATRIX matOrigin = pOrigin->GetWorld();

	//�}�e���A���̐���
	for (unsigned int OriginMaterial = 0; OriginMaterial < pOrigineMesh->Material.size(); OriginMaterial++)
	{
		//�ʂ̐���
		for (unsigned int OriginFace = 0; OriginFace < pOrigineMesh->Material[OriginMaterial].FaceInfo.size(); OriginFace++)
		{
			//�ʏ��
			FACE_INFO face = pOrigineMesh->Material[OriginMaterial].FaceInfo[OriginFace];
			
			//�ʂ̏d�S������ׂ�
			int OriginVerNum = face.Vertex.size();
		
			D3DXVECTOR3 v1, v2, v3;
			D3DXVec3TransformCoord(&v1, &face.Vertex[0].vPos, &matOrigin);
			D3DXVec3TransformCoord(&v2, &face.Vertex[1].vPos, &matOrigin);
			D3DXVec3TransformCoord(&v3, &face.Vertex[2].vPos, &matOrigin);
				
			//�d�S�̈ʒu�����߂�
			D3DXVECTOR3 Center=CenterOfGravity(v1, v2, v3);

			//�d�S����Ray���΂�
			vOrigin = Center;

			//�^�[�Q�b�g�ɂȂ�|���S���̃Z�b�g
			if (TargetSet(pOut, vOrigin, vDir) == true)
			{
				return true;
			}

			//�l�p�|���S����3�p�|���S��2�ɕ�����
			if (OriginVerNum == 4)
			{
				//�e���_�����[���h�ŕϊ�
				D3DXVECTOR3 v4;
				D3DXVec3TransformCoord(&v4, &face.Vertex[3].vPos, &matOrigin);

				//�d�S�̈ʒu�����߂�
				D3DXVECTOR3 Center = CenterOfGravity(v2, v4, v3);

				//�d�S����Ray���΂�
				vOrigin = Center;
				
				//�^�[�Q�b�g�ɂȂ�|���S���̃Z�b�g
				if (TargetSet(pOut, vOrigin, vDir) == true)
				{
					return true;
				}
			}
			
			//���_���Ƃɒ��ׂ�
			for (unsigned int OriginVer = 0; OriginVer < face.Vertex.size(); OriginVer++)
			{
				//���_�̈ʒu
				vOrigin = face.Vertex[OriginVer].vPos;

				//���_�����[���h�s��ŕϊ�
				D3DXVec3TransformCoord(&vOrigin, &vOrigin, &matOrigin);
				
				//�^�[�Q�b�g�ɂȂ�|���S���̃Z�b�g
				if (TargetSet(pOut, vOrigin, vDir) == true)
				{
					return true;
				}
			}
		}
	}
	return false;
}

//�^�[�Q�b�g�ɂȂ�|���S�������Z�b�g����
bool CRay::TargetSet(OutData* pOut,D3DXVECTOR3 vOrigin, D3DXVECTOR3 vDir)
{
	//�o�^����Ă�f�[�^�̐���
	for (unsigned int DataNum = 0; DataNum < m_Data.size(); DataNum++)
	{
		MY_MESH* pMesh = m_Data[DataNum]->GetMesh();
		D3DXMATRIX matWorld = m_Data[DataNum]->GetWorld();

		//�}�e���A������
		for (unsigned int MaterialNum = 0; MaterialNum < pMesh->Material.size(); MaterialNum++)
		{
			//�ʂ̐�
			for (unsigned int FaceNum = 0; FaceNum < pMesh->Material[MaterialNum].FaceInfo.size(); FaceNum++)
			{
				FACE_INFO face_info = pMesh->Material[MaterialNum].FaceInfo[FaceNum];

				//�ʂ̒��_�̐�
				int FaceOfVer = face_info.Vertex.size();

				//�@��
				D3DXVECTOR3 vNorm;

				//��_
				D3DXVECTOR3 vInter;
								
				//�e���_�����[���h�ŕϊ�
				D3DXVECTOR3 v1, v2, v3;
				D3DXVec3TransformCoord(&v1, &face_info.Vertex[0].vPos, &matWorld);
				D3DXVec3TransformCoord(&v2, &face_info.Vertex[1].vPos, &matWorld);
				D3DXVec3TransformCoord(&v3, &face_info.Vertex[2].vPos, &matWorld);

				//Ray����
				if (TriangleRay(&vInter, &vNorm, vOrigin, vDir, v1, v2, v3) == true)
				{
					//�o�̓f�[�^
					if (pOut != NULL)
					{
						//��_
						pOut->m_vInter = vInter;

						//�@��
						pOut->m_vRub = WallShear(&vDir, &vNorm);

						//����
						pOut->m_vRef = Reflection(&vDir, &vNorm);

						//�Փ˖�
						pOut->m_HitFace = face_info;

						//�Փ˖ʂ̖@��
						pOut->m_vNorm = vNorm;
					}

					return true;
				}
				
				//�l�p�|���S���͎O�p�|���S��2�ɕ�����
				if(FaceOfVer==4)
				{
					//�e���_�����[���h�ŕϊ�
					D3DXVECTOR3 v4;
					D3DXVec3TransformCoord(&v4, &face_info.Vertex[3].vPos, &matWorld);
					
					//Ray����
					if (TriangleRay(&vInter, &vNorm, vOrigin, vDir, v2, v4, v3) == true)
					{
						//�o�̓f�[�^
						if (pOut != NULL)
						{
							//��_
							pOut->m_vInter = vInter;

							//�@��
							pOut->m_vRub = WallShear(&vDir, &vNorm);

							//����
							pOut->m_vRef = Reflection(&vDir, &vNorm);

							//�Փ˖�
							pOut->m_HitFace = face_info;

							//�Փ˖ʂ̖@��
							pOut->m_vNorm = vNorm;
						}
						return true;
					}
				}
			}
		}
	}
	return false;
}

//Ray�̔���
bool CRay::TriangleRay(D3DXVECTOR3* OutPoint,D3DXVECTOR3* OutNorm ,D3DXVECTOR3 vRayOrigin, D3DXVECTOR3 vRayDir, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC)
{
	//�ʂ̒��_����Ray�̎n�_�ƏI�_�ւ̃x�N�g��
	D3DXVECTOR3 vStart, vEnd;
	vStart = vA - vRayOrigin;
	vEnd = vA - (vRayOrigin + vRayDir);

	//�e�ӂւ̃x�N�g��
	D3DXVECTOR3 vAB, vBC, vCA;
	vAB = vB - vA;
	vBC = vC - vB;
	vCA = vA - vC;

	//�@���x�N�g�������߂�
	D3DXVECTOR3 vNorm;
	D3DXVec3Cross(&vNorm, &vAB, &vBC);
	D3DXVec3Normalize(&vNorm,&vNorm);

	//�o�͗p�@��
	OutNorm->x = vNorm.x;
	OutNorm->y = vNorm.y;
	OutNorm->z = vNorm.z;

	//�@���Ǝn�_�I�_�ւ̃x�N�g��
	float dotStart = D3DXVec3Dot(&vStart, &vNorm);
	float dotEnd = D3DXVec3Dot(&vEnd, &vNorm);
			
	//Ray�̎n�_�ƏI�_�����ʏ�ɂ���A��_���v�Z�ł��Ȃ��B
	if (dotStart == 0.0f && dotEnd == 0.0f)
	{
		return false;
	}

	//�n�_�ƏI�_�����������ɂ���
	if (dotStart > 0.0f && dotEnd > 0.0f || dotStart < 0.0f && dotEnd < 0.0f)
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
		float a1 = vRayDir.x;
		float b1 = vRayDir.y;
		float c1 = vRayDir.z;
		float x1 = vRayOrigin.x;
		float y1 = vRayOrigin.y;
		float z1 = vRayOrigin.z;
		float t = -(a*x1 + b * y1 + c * z1 + d) / (a*a1 + b * b1 + c * c1);

		//�����������_�����߂�
		D3DXVECTOR3 vPoint;
		vPoint.x = x1 + a1 * t;
		vPoint.y = y1 + b1 * t;
		vPoint.z = z1 + c1 * t;
		
		//��_�Ƃ��ꂼ��̕ӂ̊O�ς��@���Ɠ����������m��ׂ�

		//��_����e���_�ւ̃x�N�g��
		D3DXVECTOR3 vPA, vPB, vPC;
		vPA = vA - vPoint;		
		vPB = vB - vPoint;
		vPC = vC - vPoint;
				
		//��_����A�ւ̃x�N�g����AB�̊O��
		D3DXVECTOR3 vCrossAB;
		D3DXVec3Cross(&vCrossAB, &vPA, &vAB);

		//��_����B�ւ̃x�N�g����BC�̊O��
		D3DXVECTOR3 vCrossBC;
		D3DXVec3Cross(&vCrossBC, &vPB, &vBC);
		
		//��_����C�ւ̃x�N�g����CA�̊O��
		D3DXVECTOR3 vCrossCA;
		D3DXVec3Cross(&vCrossCA, &vPC, &vCA);
		
		//�O�ςƖ@���̓���
		float dot1, dot2, dot3;
		dot1 = D3DXVec3Dot(&vCrossAB, &vNorm);
		dot2 = D3DXVec3Dot(&vCrossBC, &vNorm);
		dot3 = D3DXVec3Dot(&vCrossCA, &vNorm);

		//�S�Ă̊O�ς��@���Ɠ��������Ȃ瓖�����Ă�
		if (dot1 > 0.0f && dot2 > 0.0f && dot3 > 0.0f)
		{
			//��_
			OutPoint->x = vPoint.x;
			OutPoint->y = vPoint.y;
			OutPoint->z = vPoint.z;

			return true;
		}
	}
	return false;
}

//���
void CRay::Release()
{
	VectorRelease(m_Data);

	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}

//3�p�|���S���̏d�S�̈ʒu�����߂�
D3DXVECTOR3 CRay::CenterOfGravity(D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3)
{
	D3DXVECTOR3 Out;
	Out.x = (v1.x + v2.x + v3.x) / 3;
	Out.y = (v1.y + v2.y + v3.y) / 3;
	Out.z = (v1.z + v2.z + v3.z) / 3;

	return Out;
}

//�ǂ���
D3DXVECTOR3 CRay::WallShear( D3DXVECTOR3* Front, D3DXVECTOR3* Norm)
{
	//�@�����K��
	D3DXVECTOR3 Normal_n;
	D3DXVec3Normalize(&Normal_n, Norm);

	//�ǂ���x�N�g��
	D3DXVECTOR3 Shear = (*Front) - D3DXVec3Dot(Front, &Normal_n) * Normal_n;
	
	//���K���ǂ���x�N�g��
	D3DXVECTOR3 Shear_norm;
	D3DXVec3Normalize(&Shear_norm, &Shear);


	return Shear_norm;
}

//���˃x�N�g��
D3DXVECTOR3 CRay::Reflection(D3DXVECTOR3* Front, D3DXVECTOR3* Norm)
{
	//�@��������
	D3DXVECTOR3 Normal_n;
	D3DXVec3Normalize(&Normal_n,Norm);

	//���˃x�N�g��
	D3DXVECTOR3 Rub = (*Front) - 2.0f * D3DXVec3Dot(Front, &Normal_n) * Normal_n;
	
	//���ˏ�����
	D3DXVECTOR3 Rub_norm;
	D3DXVec3Normalize(&Rub_norm, &Rub);
	
	return Rub_norm;
}
