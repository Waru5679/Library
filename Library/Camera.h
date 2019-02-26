#pragma once

//�p���p�@�J�����N���X
class Camera
{
public:
	virtual void Init()=0;	//������
	virtual void Update()=0;	//�X�V

	D3DXMATRIX GetWorldMatrix() { return m_matWorld; }
	D3DXMATRIX GetViewMatrix() { return m_matView; }
	D3DXMATRIX GetProjMatrix() { return m_matProj; }
	D3DXVECTOR3 GetEye() { return m_vEye; }
	D3DXVECTOR3 GetAngle() { return m_vAngle; }
	D3DXVECTOR3 GetVertical() { return m_vVertical; }

protected:
	D3DXVECTOR3 m_vEye;	//���_
	D3DXVECTOR3 m_vLook;//�����_
	D3DXVECTOR3 m_vUp;	//�J�����̌���
	
	D3DXVECTOR3 m_vAngle;	//�p�x
	D3DXVECTOR3 m_vVertical;//�J�����ɂɐ����ɂȂ�p�x(�QD�`��p)

	D3DXMATRIX m_matView;	//�r���[�}�g���b�N�X
	D3DXMATRIX m_matProj;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_matWorld;	//���[���h�}�g���b�N�X
};