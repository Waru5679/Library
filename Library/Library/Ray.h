#pragma once

//���C�u�����t�@�C��
#include "Main.h"
#include "CObj.h"
#include "Hit.h"
#include <vector>
using namespace std;

//�o�̓f�[�^
struct OutData
{
	D3DXVECTOR3 m_vRef;		//����
	D3DXVECTOR3 m_vRub;		//�ǂ���
	D3DXVECTOR3 m_vInter;	//��_
};

//���C�̔���p�N���X
class CRay
{
public:
	void Release();	//���

	//�o�^
	void Insert(CObj3DBase* pObj);

	//Ray������Ăяo��
	bool RayHit(OutData* pOut, CObj3DBase* pOrigin, D3DXVECTOR3 vDir, int Id);

private:
	//Ray�̔���
	bool TriangleRay(D3DXVECTOR3* OutPoint, D3DXVECTOR3* OutNorm,D3DXVECTOR3 vRayOrigin, D3DXVECTOR3 vRayDir, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC);

	//�ǎC��
	D3DXVECTOR3 WallShear(D3DXVECTOR3* Front, D3DXVECTOR3* Norm);

	//���˃x�N�g��
	D3DXVECTOR3 Reflection(D3DXVECTOR3* Front, D3DXVECTOR3* Norm);

	vector<CObj3DBase*> m_Data;//Ray������s�����X�g
};

extern CRay g_Ray;