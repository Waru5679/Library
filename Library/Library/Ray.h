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
	bool RayHit(D3DXVECTOR3* OutPoint,D3DXVECTOR3 Pos, D3DXVECTOR3 RayVec);

	//��_���擾����
	D3DXVECTOR3 GetShear(){ return m_vShear; }
private:
	//Ray�̔���(�l�p)
	bool SquareRay(D3DXVECTOR3* OutPoint, D3DXVECTOR3 RayPos, D3DXVECTOR3 RayVec, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC, D3DXVECTOR3 vD);

	//Ray�̔���(�O�p)
	bool TriangleRay(D3DXVECTOR3* OutPoint, D3DXVECTOR3 RayPos, D3DXVECTOR3 RayVec, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC);

	//�ǂ���
	D3DXVECTOR3* WallShear(D3DXVECTOR3* Out, D3DXVECTOR3 Front, D3DXVECTOR3 Normal);

	D3DXVECTOR3 m_vNorm;	//�@��
	D3DXVECTOR3 m_vShear;	//�Փˎ��̌�_

	vector<CObj3D*> m_Data;//Ray������s�����X�g
};

extern CRay g_Ray;