#pragma once

//���C�u�����t�@�C��
#include "../Library/Main.h"
#include "../Library/CObj.h" 
#include "../Library/Hit.h"

#include "Player.h"

constexpr int DELETE_TIME{ 300 };

//�e�N���X
class CBullet :public CObj3DBase
{
public:
	//�R���X�g���N�^
	CBullet(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale);
	~CBullet() {};

	void Init();	//������
	void Update();	//�X�V
	void Draw();	//�`��

	D3DXVECTOR3 GetPos() { return m_vPos; }
	D3DXVECTOR3 GetAngle() { return m_vAngle; }

private:

	int m_Count;				//�J�E���^�[
	D3DXVECTOR3 m_vMove;		//�ړ��x�N�g��
	CollisionData m_Collision;	//�����蔻��	
	float m_fSpeed;				//�ړ��X�s�[�h
	CPlayer* m_pPlayer;			//�v���C���[�̃|�C���^
	D3DXVECTOR3 m_vPlayerPos;	//�v���C���[�̈ʒu
};
