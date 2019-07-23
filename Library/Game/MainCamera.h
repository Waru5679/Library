#pragma once

//���C�u�����t�@�C��
#include "../Library/Camera.h"

//�Q�[���t�@�C��
#include "Player.h"

//���C���J�����N���X
class CMainCamera :public CCameraBase
{
public:
	void Init();	//������
	void Update();	//�X�V
	void Draw() {};	//�`��
	void Release();	//�J��

private:
	D3DXVECTOR3 m_vPlayerPos;	//�v���C���[�̈ʒu
	D3DXVECTOR3 m_vPlayerFront;	//�v���C���[�̐��ʕ���
	
	float m_fCameraDis;	//�v���C���[�ƃJ�����̋���
	float m_fLookDis;	//�v���C���[�ƒ����_�̋���

	D3DXMATRIX m_matRot;	//��]�s��
};