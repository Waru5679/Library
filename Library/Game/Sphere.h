#pragma once

//���C�u�����t�@�C��
#include "../Library/LibraryHeader.h"
#include "../Library/CObj.h"

//���N���X
class CSphere:public CObj3DBase
{
public:
	//�R���X�g���N�^
	CSphere(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale);

	void Init();	//������
	void Update();	//�X�V
	void Draw();	//�`��
private:
	float m_fRadius;	//���̔��a

	CollisionData m_Collision;//�����蔻��p
};