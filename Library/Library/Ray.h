#pragma once

//���C�u�����t�@�C��
#include "Main.h"
#include "Obb.h"
#include <vector>
using namespace std;

//���C�̔���p�N���X
class CRay
{
public:
	//�o�^
	void Insert(CObj3D* pObj);

	//Ray������Ăяo��
	bool RayHit(D3DXVECTOR3*OutPoint, CObj3D* pOrigin, D3DXVECTOR3 vDir, int Id);

private:
	//Ray�̔���(�l�p)
	bool SquareRay(D3DXVECTOR3* OutPoint, D3DXVECTOR3 RayPos, D3DXVECTOR3 RayVec, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC, D3DXVECTOR3 vD, D3DXVECTOR3 vNorm);

	//Ray�̔���(�O�p)
	bool TriangleRay(D3DXVECTOR3* OutPoint,D3DXVECTOR3 vRayOrigin, D3DXVECTOR3 vRayDir, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC);

	//�ǎC��
	D3DXVECTOR3 WallShear(D3DXVECTOR3 Front, D3DXVECTOR3 Normal);


	vector<CObj3D*> m_Data;//Ray������s�����X�g
};

extern CRay g_Ray;