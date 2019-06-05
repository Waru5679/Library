#include "Ray.h"
#include "Math.h"
CRay g_Ray;

//�o�^
void CRay::Insert(CObj3D* pObj)
{
	m_Data.push_back(pObj);
}

//Ray�̓����蔻��
bool CRay::RayHit(D3DXVECTOR3*OutPoint, CObj3D* pOrigin,D3DXVECTOR3 vDir,int Id)
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
			
			//���_���Ƃɒ��ׂ�
			for (unsigned int OriginVer = 0; OriginVer < face.Vertex.size(); OriginVer++)
			{
				//���_�̈ʒu
				vOrigin = face.Vertex[OriginVer].vPos;

				//���_�����[���h�s��ŕϊ�
				D3DXVec3TransformCoord(&vOrigin, &vOrigin, &matOrigin);

				//�@���̕�����
				D3DXVECTOR3 vFaceNorm = face.Vertex[OriginVer].vNorm;

				//Ray�x�N�g���ƒ��_�̖@���̓���
				float dot = D3DXVec3Dot(&vDir, &vFaceNorm);

				if (dot < 0.0f)
				{
					//���ς��}�C�i�X�Ȃ瓖����Ȃ�
					break;
				}

				//�o�^����Ă�f�[�^�̐���
				for (unsigned int DataNum = 0; DataNum < m_Data.size(); DataNum++)
				{
					//�w�肵���I�u�W�F�N�g����
					if (m_Data[DataNum]->m_id == Id)
					{
						MY_MESH* pTargetMesh = m_Data[DataNum]->GetMesh();
						D3DXMATRIX matTarget = m_Data[DataNum]->GetWorld();

						//�}�e���A������
						for (unsigned int TargetMaterial = 0; TargetMaterial < pTargetMesh->Material.size(); TargetMaterial++)
						{
							//�ʂ̐�
							for (unsigned int TargetFace = 0; TargetFace < pTargetMesh->Material[TargetMaterial].FaceInfo.size(); TargetFace++)
							{
								FACE_INFO Target = pTargetMesh->Material[TargetMaterial].FaceInfo[TargetFace];

								//�ʂ̒��_�̐�
								int FaceOfVer = Target.Vertex.size();

								//�O�p�|���S��
								if (FaceOfVer == 3)
								{
									//�e���_�����[���h�ŕϊ�
									D3DXVECTOR3 v1, v2, v3;
									D3DXVec3TransformCoord(&v1, &Target.Vertex[0].vPos, &matTarget);
									D3DXVec3TransformCoord(&v2, &Target.Vertex[1].vPos, &matTarget);
									D3DXVec3TransformCoord(&v3, &Target.Vertex[2].vPos, &matTarget);

									//Ray����
									if (TriangleRay(OutPoint, vOrigin, vDir, v1, v2, v3) == true)
									{
										return true;
									}
								}
								//�l�p�|���S��
								else
								{
									//�e���_�����[���h�ŕϊ�
									D3DXVECTOR3 v1, v2, v3,v4;
									D3DXVec3TransformCoord(&v1, &Target.Vertex[0].vPos, &matTarget);
									D3DXVec3TransformCoord(&v2, &Target.Vertex[1].vPos, &matTarget);
									D3DXVec3TransformCoord(&v3, &Target.Vertex[2].vPos, &matTarget);
									D3DXVec3TransformCoord(&v4, &Target.Vertex[3].vPos, &matTarget);

									//�O�p�|���S��2�ɕ������Ē��ׂ�

									//1�ڂ̎O�p�|���S��Ray����
									if (TriangleRay(OutPoint, vOrigin, vDir, v1, v2, v3) == true)
									{
										return true;
									}

									//2�ڂ̎O�p�|���S��Ray����
									if (TriangleRay(OutPoint, vOrigin, vDir, v2, v4, v3) == true)
									{
										return true;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}

//Ray�̔���
bool CRay::TriangleRay(D3DXVECTOR3* OutPoint, D3DXVECTOR3 vRayOrigin, D3DXVECTOR3 vRayDir, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC)
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
			OutPoint = &vPoint;
			return true;
		}
	}
	return false;
}

//���
void CRay::Release()
{
	VectorRelease(m_Data);
}

//
////�ǂ���
//D3DXVECTOR3 CRay::WallShear( D3DXVECTOR3 Front, D3DXVECTOR3 Normal)
//{
//	//�@�����K��
//	D3DXVECTOR3 Normal_n;
//	D3DXVec3Normalize(&Normal_n, &Normal);
//
//	//�ǂ���x�N�g��
//	D3DXVECTOR3 Shear = Front - D3DXVec3Dot(&Front, &Normal_n) * Normal_n;
//	
//	//���K���ǂ���x�N�g��
//	D3DXVECTOR3 Shear_norm;
//	D3DXVec3Normalize(&Shear_norm, &Shear);
//
//	return Shear_norm;
//}
