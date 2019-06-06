#pragma once
#include "Main.h"
#include <vector>
using namespace std;

//���f�[�^
struct SphereData
{
	D3DXVECTOR3 m_vPos;	//�ʒu
	float m_fRadius;	//���a
};

//�����蔻��N���X
class CHit
{
public:
	SphereData Create(D3DXVECTOR3 vPos, float fRadius,D3DXVECTOR3 vScale);			//�o�^
	void Insert(SphereData* pData);
	bool SphereHit();
	void UpData(SphereData* pData, D3DXVECTOR3 vPos);
private:
	bool SphereAndSphre(SphereData* data1, SphereData* data2);//�q�b�g����
	vector<SphereData*> m_data;	//���f�[�^
};

extern CHit g_Hit;