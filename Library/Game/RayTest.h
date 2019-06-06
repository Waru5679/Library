#pragma once

//���C�u�����t�@�C��
#include "../Library/Main.h"
#include "../Library/CObj.h" 

//Ray�̃e�X�g�N���X
class CRayTest :public CObj3DBase
{
public:
	//�R���X�g���N�^
	CRayTest(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale);
	~CRayTest() {};

	void Init();	//������
	void Update();	//�X�V
	void Draw();	//�`��

	D3DXVECTOR3 GetPos() { return m_vPos; }
	D3DXVECTOR3 GetAngle() { return m_vAngle; }
	
private:
};

#pragma once
