#pragma once
#include "Main.h"
#include "Obb.h"

#define UP 4
class CRay
{
public:
	//Ray������Ăяo��
	bool RayHit(D3DXVECTOR3* OutPoint,D3DXVECTOR3 Pos, D3DXVECTOR3 RayVec, OBB_Data Obb);

	D3DXVECTOR3 GetShear(){ return m_vShear; }
private:
	//Ray�̔���
	bool Ray(D3DXVECTOR3* OutPoint, D3DXVECTOR3 RayPos, D3DXVECTOR3 RayVec, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC, D3DXVECTOR3 vD);

	//�ǂ���
	D3DXVECTOR3* WallShear(D3DXVECTOR3* Out, D3DXVECTOR3 Front, D3DXVECTOR3 Normal);

	//�@��
	D3DXVECTOR3 m_vNorm;
	D3DXVECTOR3 m_vShear;
};

extern CRay g_Ray;