#pragma once

//���C�u�����t�@�C��
#include "../Library/Main.h"
#include "../Library/CObj.h" 
#include "../Library/Obb.h"
#include "../Library/Hit.h"

//�v���C���[�N���X
class CPlayer:public CObj3D
{
public:
	//�R���X�g���N�^
	CPlayer(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale);
	~CPlayer() {};

	void Init();	//������
	void Update();	//�X�V
	void Draw();	//�`��
	void Input();	//����

	D3DXVECTOR3 GetPos() { return m_vPos; }
	D3DXVECTOR3 GetAngle() { return m_vAngle; }
	D3DXVECTOR3 GetDir() { return m_vLastMove; }
	
private:
	D3DXVECTOR3 m_vMove;	//�ړ��x�N�g��
	D3DXVECTOR3 m_vLastMove;//�Ō�Ɉړ���������
	
	float m_fSpeed;	//�ړ��X�s�[�h
	bool m_bHit;	//�q�b�g�e�X�g

	OBB_Data m_Obb;			//OBB�f�[�^
	SphereData m_SphereData;	//���f�[�^
};

