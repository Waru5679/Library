#pragma once
#include "../Library/CObj.h"
#include "Player.h"
//�Ə��N���X
class CAiming :public CObj3DBase
{
public:
	//�R���X�g���N�^
	CAiming(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale);
	void Init();	//�`��
	void Update();	//�X�V
	void Draw();	//�`��
	void Release();	//�J��
private:
	CPlayer* m_pPlayer;	//�v���C���[�̃|�C���^
};