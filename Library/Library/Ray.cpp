#include "Ray.h"
#include "Math.h"
#include "Release.h"

//インスタンス
CRay* CRay::m_pInstance = nullptr;

//登録
void CRay::Insert(CObj3DBase* pObj)
{
	m_Data.push_back(pObj);
}

//RayのHit判定
bool CRay::RayHit(OutData* pOut, CObj3DBase* pOrigin, D3DXVECTOR3 vDir)
{
	return OriginSet(pOut, pOrigin, vDir);
}

//Rayの発射点を決める
bool CRay::OriginSet(OutData* pOut,CObj3DBase* pOrigin,D3DXVECTOR3 vDir)
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
			
			//面の重心をしらべる
			int OriginVerNum = face.Vertex.size();
		
			D3DXVECTOR3 v1, v2, v3;
			D3DXVec3TransformCoord(&v1, &face.Vertex[0].vPos, &matOrigin);
			D3DXVec3TransformCoord(&v2, &face.Vertex[1].vPos, &matOrigin);
			D3DXVec3TransformCoord(&v3, &face.Vertex[2].vPos, &matOrigin);
				
			//重心の位置を求める
			D3DXVECTOR3 Center=CenterOfGravity(v1, v2, v3);

			//重心からRayを飛ばす
			vOrigin = Center;

			//ターゲットになるポリゴンのセット
			if (TargetSet(pOut, vOrigin, vDir) == true)
			{
				return true;
			}

			//四角ポリゴンは3角ポリゴン2つに分ける
			if (OriginVerNum == 4)
			{
				//各頂点をワールドで変換
				D3DXVECTOR3 v4;
				D3DXVec3TransformCoord(&v4, &face.Vertex[3].vPos, &matOrigin);

				//重心の位置を求める
				D3DXVECTOR3 Center = CenterOfGravity(v2, v4, v3);

				//重心からRayを飛ばす
				vOrigin = Center;
				
				//ターゲットになるポリゴンのセット
				if (TargetSet(pOut, vOrigin, vDir) == true)
				{
					return true;
				}
			}
			
			//頂点ごとに調べる
			for (unsigned int OriginVer = 0; OriginVer < face.Vertex.size(); OriginVer++)
			{
				//頂点の位置
				vOrigin = face.Vertex[OriginVer].vPos;

				//頂点をワールド行列で変換
				D3DXVec3TransformCoord(&vOrigin, &vOrigin, &matOrigin);
				
				//ターゲットになるポリゴンのセット
				if (TargetSet(pOut, vOrigin, vDir) == true)
				{
					return true;
				}
			}
		}
	}
	return false;
}

//ターゲットになるポリゴン情報をセットする
bool CRay::TargetSet(OutData* pOut,D3DXVECTOR3 vOrigin, D3DXVECTOR3 vDir)
{
	//登録されてるデータの数回す
	for (unsigned int DataNum = 0; DataNum < m_Data.size(); DataNum++)
	{
		MY_MESH* pMesh = m_Data[DataNum]->GetMesh();
		D3DXMATRIX matWorld = m_Data[DataNum]->GetWorld();

		//マテリアル分回す
		for (unsigned int MaterialNum = 0; MaterialNum < pMesh->Material.size(); MaterialNum++)
		{
			//面の数
			for (unsigned int FaceNum = 0; FaceNum < pMesh->Material[MaterialNum].FaceInfo.size(); FaceNum++)
			{
				FACE_INFO face_info = pMesh->Material[MaterialNum].FaceInfo[FaceNum];

				//面の頂点の数
				int FaceOfVer = face_info.Vertex.size();

				//法線
				D3DXVECTOR3 vNorm;

				//交点
				D3DXVECTOR3 vInter;
								
				//各頂点をワールドで変換
				D3DXVECTOR3 v1, v2, v3;
				D3DXVec3TransformCoord(&v1, &face_info.Vertex[0].vPos, &matWorld);
				D3DXVec3TransformCoord(&v2, &face_info.Vertex[1].vPos, &matWorld);
				D3DXVec3TransformCoord(&v3, &face_info.Vertex[2].vPos, &matWorld);

				//Ray判定
				if (TriangleRay(&vInter, &vNorm, vOrigin, vDir, v1, v2, v3) == true)
				{
					//出力データ
					if (pOut != NULL)
					{
						//交点
						pOut->m_vInter = vInter;

						//法線
						pOut->m_vRub = WallShear(&vDir, &vNorm);

						//反射
						pOut->m_vRef = Reflection(&vDir, &vNorm);

						//衝突面
						pOut->m_HitFace = face_info;

						//衝突面の法線
						pOut->m_vNorm = vNorm;
					}

					return true;
				}
				
				//四角ポリゴンは三角ポリゴン2つに分ける
				if(FaceOfVer==4)
				{
					//各頂点をワールドで変換
					D3DXVECTOR3 v4;
					D3DXVec3TransformCoord(&v4, &face_info.Vertex[3].vPos, &matWorld);
					
					//Ray判定
					if (TriangleRay(&vInter, &vNorm, vOrigin, vDir, v2, v4, v3) == true)
					{
						//出力データ
						if (pOut != NULL)
						{
							//交点
							pOut->m_vInter = vInter;

							//法線
							pOut->m_vRub = WallShear(&vDir, &vNorm);

							//反射
							pOut->m_vRef = Reflection(&vDir, &vNorm);

							//衝突面
							pOut->m_HitFace = face_info;

							//衝突面の法線
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

//Rayの判定
bool CRay::TriangleRay(D3DXVECTOR3* OutPoint,D3DXVECTOR3* OutNorm ,D3DXVECTOR3 vRayOrigin, D3DXVECTOR3 vRayDir, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC)
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

	//出力用法線
	OutNorm->x = vNorm.x;
	OutNorm->y = vNorm.y;
	OutNorm->z = vNorm.z;

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
			OutPoint->x = vPoint.x;
			OutPoint->y = vPoint.y;
			OutPoint->z = vPoint.z;

			return true;
		}
	}
	return false;
}

//解放
void CRay::Release()
{
	VectorRelease(m_Data);

	//インスタンス破棄
	PointerRelease(m_pInstance);
}

//3角ポリゴンの重心の位置を求める
D3DXVECTOR3 CRay::CenterOfGravity(D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3)
{
	D3DXVECTOR3 Out;
	Out.x = (v1.x + v2.x + v3.x) / 3;
	Out.y = (v1.y + v2.y + v3.y) / 3;
	Out.z = (v1.z + v2.z + v3.z) / 3;

	return Out;
}

//壁ずり
D3DXVECTOR3 CRay::WallShear( D3DXVECTOR3* Front, D3DXVECTOR3* Norm)
{
	//法線正規化
	D3DXVECTOR3 Normal_n;
	D3DXVec3Normalize(&Normal_n, Norm);

	//壁ずりベクトル
	D3DXVECTOR3 Shear = (*Front) - D3DXVec3Dot(Front, &Normal_n) * Normal_n;
	
	//正規化壁ずりベクトル
	D3DXVECTOR3 Shear_norm;
	D3DXVec3Normalize(&Shear_norm, &Shear);


	return Shear_norm;
}

//反射ベクトル
D3DXVECTOR3 CRay::Reflection(D3DXVECTOR3* Front, D3DXVECTOR3* Norm)
{
	//法線初期化
	D3DXVECTOR3 Normal_n;
	D3DXVec3Normalize(&Normal_n,Norm);

	//反射ベクトル
	D3DXVECTOR3 Rub = (*Front) - 2.0f * D3DXVec3Dot(Front, &Normal_n) * Normal_n;
	
	//反射初期化
	D3DXVECTOR3 Rub_norm;
	D3DXVec3Normalize(&Rub_norm, &Rub);
	
	return Rub_norm;
}
