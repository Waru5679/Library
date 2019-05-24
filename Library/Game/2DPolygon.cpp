//���C�u�����t�@�C��
#include "../Library/Font.h"
#include "../Library/Input.h"

//�Q�[���t�@�C��
#include "2DPolygon.h"

//������
void C2DPolygon::Init()
{
	//�ʒu
	m_ScreenPos.x = 0.0f;
	m_ScreenPos.y = 0.0f;
	
	//�ړ�
	m_Move.x = 0.0f;
	m_Move.y = 0.0f;

	//��]
	m_fRad = 0.0f;

	//�`��F
	m_fColor[0] = 1.0f;
	m_fColor[1] = 1.0f;
	m_fColor[2] = 1.0f;
	m_fColor[3] = 0.5f;

}

//�X�V
void C2DPolygon::Update()
{
	//�ړ��e�X�g
	if (g_input.GetKeyPush(VK_RIGHT)==true)
	{
		m_Move.x = 1.0f;
	}
	if (g_input.GetKeyPush(VK_LEFT) == true)
	{
		m_Move.x = -1.0f;
	}
	if (g_input.GetKeyPush(VK_UP) == true)
	{
		m_Move.y = -1.0f;
	}
	if (g_input.GetKeyPush(VK_DOWN) == true)
	{
		m_Move.y = 1.0f;
	}

	//��]�e�X�g
	if (g_input.GetKeyPush(VK_RETURN) == true)
	{
		m_fRad += 1.0f;
	}

	//�ړ�
	m_ScreenPos.x += m_Move.x;
	m_ScreenPos.y += m_Move.y;

	//�ړ�������
	m_Move.x = 0.0f;
	m_Move.y = 0.0f;
}

//�`��
void C2DPolygon::Draw()
{
	//�؂���ʒu
	m_Src.m_top =200.0f;
	m_Src.m_left = 200.0f;
	m_Src.m_right = m_Src.m_left + 56.0f;
	m_Src.m_bottom = m_Src.m_top + 56.0f;
	
	//�`��ʒu
	m_Dst.m_top = m_ScreenPos.y;
	m_Dst.m_left = m_ScreenPos.x;
	m_Dst.m_right = m_Dst.m_left + 100.0f;
	m_Dst.m_bottom = m_Dst.m_top + 100.0f;
	
	//�`��
	g_Draw.Draw2D(0, &m_Src, &m_Dst, m_fColor, m_fRad);
}