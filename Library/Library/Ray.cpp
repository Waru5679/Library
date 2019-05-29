#include "Ray.h"
CRay g_Ray;

//登録
void CRay::Insert(CObj3D* pObj)
{
	m_Data.push_back(pObj);
}

//Rayの当たり判定
bool CRay::RayHit(D3DXVECTOR3* OutPoint, D3DXVECTOR3 Pos, D3DXVECTOR3 RayVec)
{
	MY_MESH* pMesh;

	//Meshの数分回す
	for (unsigned int MeshNum = 0; MeshNum < m_Data.size(); MeshNum++)
	{
		pMesh = m_Data[MeshNum]->GetMesh();

		//Materialの数分回す
		for (unsigned int MaterialNum = 0; MaterialNum < pMesh->Material.size(); MaterialNum++)
		{
			//面の数だけ回す
			for (unsigned int FaceNum = 0; FaceNum < pMesh->Material[MaterialNum].FaceInfo.size(); FaceNum++)
			{
				//面情報
				FACE_INFO face = pMesh->Material[MaterialNum].FaceInfo[FaceNum];

				//頂点数
				int VerNum = face.FaceofVer;
				
				//三角ポリゴン
				if (VerNum == 3)
				{
					//Ray判定
					if (TriangleRay(OutPoint, Pos, RayVec, face.pvPos[0], face.pvPos[1],face.pvPos[2],face.vNorm) == true)
					{
						//壁すり
						WallShear(OutPoint, RayVec, face.vNorm);
						return true;
					}
				}
				//四角ポリゴン
				if (VerNum == 4)
				{
					//内積の符号が同じになって通らないっぽい
					//Ray判定
					//if (SquareRay(OutPoint, Pos, RayVec, face.pVer[0].vPos, face.pVer[1].vPos, face.pVer[3].vPos, face.pVer[2].vPos,face.vNorm) == true)
					//{
					//	//壁すり
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

//Rayの判定(三角)
bool CRay::TriangleRay(D3DXVECTOR3* OutPoint, D3DXVECTOR3 RayPos, D3DXVECTOR3 RayVec, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC,D3DXVECTOR3 vNorm)
{
	//面の頂点からRayの始点と終点へのベクトル
	D3DXVECTOR3 vStart, vEnd;
	vStart = vA - RayPos;
	vEnd = vA - (RayPos + RayVec);

	//法線と始点終点へのベクトル
	float dotStart = D3DXVec3Dot(&vStart, &vNorm);
	float dotEnd = D3DXVec3Dot(&vEnd, &vNorm);
		
	
	//Rayの始点と終点が平面上にあり、交点を計算できない。
	if (dotStart == 0.0f && dotEnd == 0.0f)
	{
		return false;
	}

	//始点と終点が同じ方向にいる
	if (dotStart > 0.0f && dotEnd > 0.0f ||
		dotStart < 0.0f && dotEnd < 0.0f)
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
		float a1 = RayVec.x;
		float b1 = RayVec.y;
		float c1 = RayVec.z;
		float x1 = RayPos.x;
		float y1 = RayPos.y;
		float z1 = RayPos.z;
		float t = -(a*x1 + b * y1 + c * z1 + d) / (a*a1 + b * b1 + c * c1);

		//直線式から交点を求める
		D3DXVECTOR3 vInterPoint;
		vInterPoint.x = x1 + a1 * t;
		vInterPoint.y = y1 + b1 * t;
		vInterPoint.z = z1 + c1 * t;


		//交点とそれぞれの辺の外積が法線と同じ向きか知らべる

		D3DXVECTOR3 vA_Inter = vA - vInterPoint;	//交点からAへのベクトル
		D3DXVECTOR3 vAB = vA - vB;					//BからAへのベクトル

		//交点とABの外積
		D3DXVECTOR3 vCrossAB;
		D3DXVec3Cross(&vCrossAB, &vA_Inter, &vAB);
		float dotAB = D3DXVec3Dot(&vCrossAB, &vNorm);

		D3DXVECTOR3 vB_Inter = vB - vInterPoint;	//交点からBへのベクトル
		D3DXVECTOR3 vBC = vB - vC;					//CからBへのベクトル

		//交点とABの外積
		D3DXVECTOR3 vCrossBC;
		D3DXVec3Cross(&vCrossBC, &vB_Inter, &vBC);
		float dotBC = D3DXVec3Dot(&vCrossBC, &vNorm);


		D3DXVECTOR3 vC_Inter = vC - vInterPoint;	//交点からCへのベクトル
		D3DXVECTOR3 vCA = vA - vC;					//AからCへのベクトル

		//交点とABの外積
		D3DXVECTOR3 vCrossCA;
		D3DXVec3Cross(&vCrossCA, &vC_Inter, &vCA);
		float dotCA = D3DXVec3Dot(&vCrossCA, &vNorm);

		//全ての外積が法線と同じ方向を向いている時交点は面の中にある
		if (dotAB > 0.0f && dotBC > 0.0f && dotCA > 0.0f)
		{
			//交点
			OutPoint = &vInterPoint;
			return true;
		}
	}
	return false;
}


//Rayの判定(四角ポリゴン)
bool CRay::SquareRay(D3DXVECTOR3* OutPoint,D3DXVECTOR3 RayPos, D3DXVECTOR3 RayVec, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC, D3DXVECTOR3 vD, D3DXVECTOR3 vNorm)
{
	//平面上の点AとからRayの始点と終点へのベクトルを求める
	D3DXVECTOR3 vA_Start = -(RayPos - vA);
	D3DXVECTOR3 vA_Final = -((RayPos + RayVec) - vA);

	//4辺のベクトルを求める
	D3DXVECTOR3 AB, BC, CD, DA;
	AB = vB - vA;
	BC = vC - vB;
	CD = vD - vC;
	DA = vA - vD;

	//平面の法線
	D3DXVECTOR3 PlaneNorm;
	D3DXVec3Cross(&PlaneNorm, &AB, &BC);

	//法線ベクトルを保持
	m_vNorm = PlaneNorm;

	//ベクトルと法線の内積
	float dot1 = D3DXVec3Dot(&vA_Start, &PlaneNorm);
	float dot2 = D3DXVec3Dot(&vA_Final, &PlaneNorm);

	//交差判定
	if (dot1 == 0.0f && dot2 == 0.0f) 
	{
		//両端が平面上にあり、交点を計算できない。
		return false;
	}
	else if ((dot1 >= 0.0f && dot2 <= 0.0f) || (dot1 <= 0.0f && dot2 >= 0.0f)) 
	{
		//内積の片方がプラスで片方がマイナスなので、交差している
		//平面の範囲内で交差しているか調べる

		//Rayの始点から各頂点へのベクトル
		D3DXVECTOR3 PA, PB, PC, PD;
		PA = vA - RayPos;
		PB = vB - RayPos;
		PC = vC - RayPos;
		PD = vD - RayPos;

		//法線を求める
		D3DXVECTOR3	nPAB, nPBC, nPCD, nPDA;
		D3DXVec3Cross(&nPAB, &PA, &AB);
		D3DXVec3Cross(&nPBC, &PB, &BC);
		D3DXVec3Cross(&nPCD, &PC, &CD);
		D3DXVec3Cross(&nPDA, &PD, &DA);

		//Rayと法線の内積を求める
		float dotPAB, dotPBC, dotPCD, dotPDA;
		dotPAB = D3DXVec3Dot(&nPAB, &RayVec);
		dotPBC = D3DXVec3Dot(&nPBC, &RayVec);
		dotPCD = D3DXVec3Dot(&nPCD, &RayVec);
		dotPDA = D3DXVec3Dot(&nPDA, &RayVec);

		//内積の符号がすべて同じなら当たってる
		if (dotPAB >= 0 && dotPBC >= 0 && dotPCD >= 0 && dotPDA >= 0 ||
			dotPAB <= 0 && dotPBC <= 0 && dotPCD <= 0 && dotPDA <= 0)
		{
			if (D3DXVec3Dot(&PlaneNorm, &RayVec) < 0)
			{
				//ポリゴン表

				//平面の方程式
				float a = PlaneNorm.x;//平面の法線
				float b = PlaneNorm.y;
				float c = PlaneNorm.z;
				float x = vA.x;	
				float y = vA.y;
				float z = vA.z;

				float d = -(a*x + b * y + c * z);

				//Rayの直線式の定数tを求める
				float a1 = RayVec.x;
				float b1 = RayVec.y;
				float c1 = RayVec.z;

				float x1 = RayPos.x;
				float y1 = RayPos.y;
				float z1 = RayPos.z;

				float t = -(a*x1 + b * y1 + c * z1 + d) / (a*a1 + b * b1 + c * c1);

				//直線式から交点を求める
				OutPoint->x = x1 + a1 * t;
				OutPoint->y = y1 + b1 * t;
				OutPoint->z = z1 + c1 * t;
			}
			return true;
		}
		else
		{
			//交差していない
			return false;
		}
	}
	else 
	{
		//交差していない
		return false;
	}	
}


//壁ずり
D3DXVECTOR3* CRay::WallShear(D3DXVECTOR3* Out, D3DXVECTOR3 Front, D3DXVECTOR3 Normal)
{
	//法線正規化
	D3DXVECTOR3 Normal_n;
	D3DXVec3Normalize(&Normal_n, &Normal);
	
	//壁ずりベクトル
	D3DXVECTOR3 Shear = Front - D3DXVec3Dot(&Front, &Normal_n) * Normal_n;
	return D3DXVec3Normalize(Out, &Shear);
}
