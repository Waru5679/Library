#pragma once

#include "CObj.h"
#include "Polygon.h"

//2D�|���S���N���X
class C2DPolygon:public CObj2D
{
public:
	void Init();	//������
	void Update();	//�X�V
	void Draw();	//�`��
private:
	RECT_F m_Dst;	//�`��ʒu
	float m_fRad;	//��]
};