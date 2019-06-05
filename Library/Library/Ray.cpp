#include "Ray.h"
#include "Math.h"
CRay g_Ray;

//登録
void CRay::Insert(CObj3D* pObj)
{
	m_Data.push_back(pObj);
}

//Rayの当たり判定
bool CRay::RayHit(D3DXVECTOR3*OutPoint, CObj3D* pOrigin,D3DXVECTOR3 vDir,int Id)
{
	//Rayを飛ばす側
	MY_MESH* pOrigineMesh = pOrigin->GetMesh();
	D3DXVECTOR3 vOrigin;
	D3DXMATRIX matOrigin = pOrigin->GetWorld();

	//マテリアルの数回す
	for (unsigned int OriginMaterial = 0; OriginMaterial < pOrigineMesh->Material.size(); OriginMaterial++)
	{
		//面の数回す
		for (unsigned int OriginFace = 0; OriginFace < pOrigineMesh->Material[OriginMaterial].FaceInfo.size(); OriginFace++)
		{
			//面情報
			FACE_INFO face = pOrigineMesh->Material[OriginMaterial].FaceInfo[OriginFace];
			
			//頂点ごとに調べる
			for (unsigned int OriginVer = 0; OriginVer < face.Vertex.size(); OriginVer++)
			{
				//頂点の位置
				vOrigin = face.Vertex[OriginVer].vPos;

				//頂点をワールド行列で変換
				D3DXVec3TransformCoord(&vOrigin, &vOrigin, &matOrigin);

				//法線の方向へ
				D3DXVECTOR3 vFaceNorm = face.Vertex[OriginVer].vNorm;

				//Rayベクトルと頂点の法線の内積
				float dot = D3DXVec3Dot(&vDir, &vFaceNorm);

				if (dot < 0.0f)
				{
					//内積がマイナスなら当たらない
					break;
				}

				//登録されてるデータの数回す
				for (unsigned int DataNum = 0; DataNum < m_Data.size(); DataNum++)
				{
					//指定したオブジェクトだけ
					if (m_Data[DataNum]->m_id == Id)
					{
						MY_MESH* pTargetMesh = m_Data[DataNum]->GetMesh();
						D3DXMATRIX matTarget = m_Data[DataNum]->GetWorld();

						//マテリアル分回す
						for (unsigned int TargetMaterial = 0; TargetMaterial < pTargetMesh->Material.size(); TargetMaterial++)
						{
							//面の数
							for (unsigned int TargetFace = 0; TargetFace < pTargetMesh->Material[TargetMaterial].FaceInfo.size(); TargetFace++)
							{
								FACE_INFO Target = pTargetMesh->Material[TargetMaterial].FaceInfo[TargetFace];

								//面の頂点の数
								int FaceOfVer = Target.Vertex.size();

								//三角ポリゴン
								if (FaceOfVer == 3)
								{
									//各頂点をワールドで変換
									D3DXVECTOR3 v1, v2, v3;
									D3DXVec3TransformCoord(&v1, &Target.Vertex[0].vPos, &matTarget);
									D3DXVec3TransformCoord(&v2, &Target.Vertex[1].vPos, &matTarget);
									D3DXVec3TransformCoord(&v3, &Target.Vertex[2].vPos, &matTarget);

									//Ray判定
									if (TriangleRay(OutPoint, vOrigin, vDir, v1, v2, v3) == true)
									{
										return true;
									}
								}
								//四角ポリゴン
								else
								{
									//各頂点をワールドで変換
									D3DXVECTOR3 v1, v2, v3,v4;
									D3DXVec3TransformCoord(&v1, &Target.Vertex[0].vPos, &matTarget);
									D3DXVec3TransformCoord(&v2, &Target.Vertex[1].vPos, &matTarget);
									D3DXVec3TransformCoord(&v3, &Target.Vertex[2].vPos, &matTarget);
									D3DXVec3TransformCoord(&v4, &Target.Vertex[3].vPos, &matTarget);

									//三角ポリゴン2つに分解して調べる

									//1つ目の三角ポリゴンRay判定
									if (TriangleRay(OutPoint, vOrigin, vDir, v1, v2, v3) == true)
									{
										return true;
									}

									//2つ目の三角ポリゴンRay判定
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

//Rayの判定
bool CRay::TriangleRay(D3DXVECTOR3* OutPoint, D3DXVECTOR3 vRayOrigin, D3DXVECTOR3 vRayDir, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC)
{
	//面の頂点からRayの始点と終点へのベクトル
	D3DXVECTOR3 vStart, vEnd;
	vStart = vA - vRayOrigin;
	vEnd = vA - (vRayOrigin + vRayDir);

	//各辺へのベクトル
	D3DXVECTOR3 vAB, vBC, vCA;
	vAB = vB - vA;
	vBC = vC - vB;
	vCA = vA - vC;

	//法線ベクトルを求める
	D3DXVECTOR3 vNorm;
	D3DXVec3Cross(&vNorm, &vAB, &vBC);
	D3DXVec3Normalize(&vNorm,&vNorm);

	//法線と始点終点へのベクトル
	float dotStart = D3DXVec3Dot(&vStart, &vNorm);
	float dotEnd = D3DXVec3Dot(&vEnd, &vNorm);
			
	//Rayの始点と終点が平面上にあり、交点を計算できない。
	if (dotStart == 0.0f && dotEnd == 0.0f)
	{
		return false;
	}

	//始点と終点が同じ方向にいる
	if (dotStart > 0.0f && dotEnd > 0.0f || dotStart < 0.0f && dotEnd < 0.0f)
	{
		return false;
	}
	//交点をもとめる
	else
	{
		//平面の方程式
		float a = vNorm.x;
		float b = vNorm.y;
		float c = vNorm.z;
		float x = vA.x;
		float y = vA.y;
		float z = vA.z;
		float d = -(a*x + b * y + c * z);

		//Rayの直線式の定数tを求める
		float a1 = vRayDir.x;
		float b1 = vRayDir.y;
		float c1 = vRayDir.z;
		float x1 = vRayOrigin.x;
		float y1 = vRayOrigin.y;
		float z1 = vRayOrigin.z;
		float t = -(a*x1 + b * y1 + c * z1 + d) / (a*a1 + b * b1 + c * c1);

		//直線式から交点を求める
		D3DXVECTOR3 vPoint;
		vPoint.x = x1 + a1 * t;
		vPoint.y = y1 + b1 * t;
		vPoint.z = z1 + c1 * t;
		
		//交点とそれぞれの辺の外積が法線と同じ向きか知らべる

		//交点から各頂点へのベクトル
		D3DXVECTOR3 vPA, vPB, vPC;
		vPA = vA - vPoint;		
		vPB = vB - vPoint;
		vPC = vC - vPoint;
				
		//交点からAへのベクトルとABの外積
		D3DXVECTOR3 vCrossAB;
		D3DXVec3Cross(&vCrossAB, &vPA, &vAB);

		//交点からBへのベクトルとBCの外積
		D3DXVECTOR3 vCrossBC;
		D3DXVec3Cross(&vCrossBC, &vPB, &vBC);
		
		//交点からCへのベクトルとCAの外積
		D3DXVECTOR3 vCrossCA;
		D3DXVec3Cross(&vCrossCA, &vPC, &vCA);
		
		//外積と法線の内積
		float dot1, dot2, dot3;
		dot1 = D3DXVec3Dot(&vCrossAB, &vNorm);
		dot2 = D3DXVec3Dot(&vCrossBC, &vNorm);
		dot3 = D3DXVec3Dot(&vCrossCA, &vNorm);

		//全ての外積が法線と同じ向きなら当たってる
		if (dot1 > 0.0f && dot2 > 0.0f && dot3 > 0.0f)
		{
			//交点
			OutPoint = &vPoint;
			return true;
		}
	}
	return false;
}

//解放
void CRay::Release()
{
	VectorRelease(m_Data);
}

//
////壁ずり
//D3DXVECTOR3 CRay::WallShear( D3DXVECTOR3 Front, D3DXVECTOR3 Normal)
//{
//	//法線正規化
//	D3DXVECTOR3 Normal_n;
//	D3DXVec3Normalize(&Normal_n, &Normal);
//
//	//壁ずりベクトル
//	D3DXVECTOR3 Shear = Front - D3DXVec3Dot(&Front, &Normal_n) * Normal_n;
//	
//	//正規化壁ずりベクトル
//	D3DXVECTOR3 Shear_norm;
//	D3DXVec3Normalize(&Shear_norm, &Shear);
//
//	return Shear_norm;
//}
