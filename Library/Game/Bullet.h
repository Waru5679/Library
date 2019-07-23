#pragma once
#include "../Library/CObj.h"

//�e�N���X
class CBullet:public CObj3DBase
{
public:
	//�R���X�g���N�^
	CBullet(D3DXVECTOR3 Pos,D3DXVECTOR3 Angle,D3DXVECTOR3 Scale,D3DXVECTOR3 Dir);
	void Init();	//������
	void Update();	//�X�V
	void Draw();	//�`��
private:
	float m_fSpeed;			//�ړ����x
	D3DXVECTOR3 m_vMove;	//�ړ��x�N�g��
	int m_Count;			//�J�E���^�[
};