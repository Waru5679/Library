#pragma once

//���C�u�����t�@�C��
#include "../Library/CObj.h"
#include "../Library/LibraryHeader.h"

//�Q�[���t�@�C��
#include "MainCamera.h"

//�v���C���[�N���X
class CPlayer:public CObj3DBase
{
public:
	//�R���X�g���N�^
	CPlayer(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale);
	~CPlayer() {};

	void Init();	//������
	void Update();	//�X�V
	void Draw();	//�`��
	void Input();	//����
	void Release();	//�J��

	D3DXVECTOR3 GetPos() { return m_vPos; }
	D3DXVECTOR3 GetAngle() { return m_vAngle; }
	D3DXVECTOR3 GetDir() { return m_vDir; }
	D3DXVECTOR3 GetLastMove() { return m_vLastMove; }
	float GetAimDis() { return m_fAimDis; }
	
private:
	D3DXVECTOR3 m_vMove;		//�ړ��x�N�g��
	D3DXVECTOR3 m_vDir;			//����
	D3DXVECTOR3 m_vLastMove;	//�Ō�Ɉړ���������
	CCollisionData m_Collision;	//�����蔻��	
	float m_fSpeed;				//�ړ��X�s�[�h
	float m_fAimDis;			//�Ə��Ƃ̋���
	bool m_bHit;				//�q�b�g�e�X�g
};