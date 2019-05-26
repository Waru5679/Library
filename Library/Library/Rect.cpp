#include "Rect.h"

//RECT_F‚ÌƒZƒbƒg
void RectSet(float x, float y, float Size_x, float Size_y, RECT_F* pOut)
{
	pOut->m_top = y;
	pOut->m_bottom = pOut->m_top + Size_y;
	pOut->m_left = x;
	pOut->m_right = pOut->m_left + Size_x;
}
