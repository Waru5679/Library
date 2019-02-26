#pragma once

#include "Main.h"
#include "Player.h"
#include "Camera.h"

class CMainCamera :public Camera
{
public:
	void Init();	//������
	void Update();	//�X�V
	
private:
	
	D3DXVECTOR3 m_vPlayerPos;		//�v���C���[�̈ʒu
	D3DXVECTOR3 m_vPlayerFront;			//�v���C���[�̐��ʕ���
	
	float m_fCameraDis;		//�v���C���[�ƃJ�����̋���
	float m_fLookDis;		//�v���C���[�ƒ����_�̋���

	D3DXMATRIX m_matRot;	//��]�s��

};