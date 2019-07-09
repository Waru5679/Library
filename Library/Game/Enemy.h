#pragma once

//���C�u�����t�@�C��
#include "../Library/Header.h"

constexpr int ATTACK_TIME{ 300 };	//�U���̊Ԋu
constexpr int ATTACK_NUM_Y{ 10 };	//�U���̐�
constexpr int ATTACK_NUM_X{ 10 };	//�U���̐�

constexpr float ATTACK_RAD_X{ 360.0f / ATTACK_NUM_X };	//�U���̊p�x
constexpr float ATTACK_RAD_Y{ 360.0f / ATTACK_NUM_Y };	//�U���̊p�x

//�G�N���X
class CEnemy :public CObj3DBase
{
public:
	//�R���X�g���N�^
	CEnemy(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale);
	~CEnemy() {};

	void Init();	//������
	void Update();	//�X�V
	void Draw();	//�`��

	D3DXVECTOR3 GetPos() { return m_vPos; }
	D3DXVECTOR3 GetAngle() { return m_vAngle; }

private:
	D3DXVECTOR3 m_vMove;		//�ړ��x�N�g��
	CollisionData m_Collision;	//�����蔻��	
	
	int m_AttackCount;			//�U���̃J�E���g
	float m_AttackLength_x;		//�U������
	float m_AttackLength_y;		//�U������

	float m_fSpeed;				//�ړ��X�s�[�h
};