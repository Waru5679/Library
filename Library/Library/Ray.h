#pragma once

//���C�u�����t�@�C��
#include "Main.h"
#include "CObj.h"
#include "Singleton.h"
#include <vector>
using namespace std;

//�o�̓f�[�^
struct OutData
{
	D3DXVECTOR3 m_vRef;		//����
	D3DXVECTOR3 m_vRub;		//�ǂ���
	D3DXVECTOR3 m_vInter;	//��_
	
	D3DXVECTOR3 m_vNorm;	//�Փ˖ʂ̖@��
	CFaceData m_HitFace;	//�Փ˖�
};

//���C�̔���p�N���X
class CRay:public CSingleton<CRay>
{
private:
	friend CSingleton<CRay>;
	CRay() {}	//�R���X�g���N�^
	~CRay() {}	//�f�X�g���N�^
public:
	void Release();	//���

	//�o�^
	void Insert(CObj3DBase* pObj);

	//Ray��Hit����
	bool RayHit(OutData* pOut, CObj3DBase* pOrigin, D3DXVECTOR3 vDir);
	
private:
	//Ray�̔���
	bool TriangleRay(D3DXVECTOR3* OutPoint, D3DXVECTOR3* OutNorm,D3DXVECTOR3 vRayOrigin, D3DXVECTOR3 vRayDir, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC);

	//3�p�|���S���̏d�S�̈ʒu�����߂�
	D3DXVECTOR3 CenterOfGravity(D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3);

	//�^�[�Q�b�g�ɂȂ�|���S�������߂�
	bool TargetSet(OutData* pOut,D3DXVECTOR3 vOrigin, D3DXVECTOR3 vDir);
	
	//Ray���˓_�����߂�
	bool OriginSet(OutData* pOut, CObj3DBase* pOrigin, D3DXVECTOR3 vDir);

	//�ǎC��
	D3DXVECTOR3 WallShear(D3DXVECTOR3* Front, D3DXVECTOR3* Norm);

	//���˃x�N�g��
	D3DXVECTOR3 Reflection(D3DXVECTOR3* Front, D3DXVECTOR3* Norm);

	vector<CObj3DBase*> m_Data;//Ray������s�����X�g
};