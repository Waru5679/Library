#pragma once

//Rect�\����
struct RECT_F
{
	float m_top;
	float m_left;
	float m_right;
	float m_bottom;
};

//RECT_F�̃Z�b�g
void RectSet(float x, float y, float Size_x, float Size_y, RECT_F* pOut);
