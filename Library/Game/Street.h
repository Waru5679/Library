#pragma once

//���C�u�����t�@�C��
#include "../Library/CObj.h"
#include "../Library/Obb.h"

//���N���X
class CStreet :public CObj3DBase
{
public:
	//�R���X�g���N�^
	CStreet(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale);
	
	void Init();	//������
	void Update();	//�X�V
	void Draw();	//�`��
private:
};