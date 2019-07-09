#pragma once

//���C�u�����t�@�C��
#include "../Library/CObj.h"
#include "../Library/LibraryHeader.h"

//2D�|���S���N���X
class C2DPolygon:public CObj2DBase
{
public:
	void Init();	//������
	void Update();	//�X�V
	void Draw();	//�`��
private:
	ColorData m_Color;	//�`��F
	RECT_F m_Src;		//�؂���ʒu
	RECT_F m_Dst;		//�`��ʒu
	float m_fRad;		//��]
};