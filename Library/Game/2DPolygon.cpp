//���C�u�����t�@�C��
#include "../Library/LibraryHeader.h"

//�Q�[���t�@�C��
#include "../Library/GameHeader.h"

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
	m_Color = ColorData(1.0f, 1.0f, 1.0f, 0.5f);
}

//�X�V
void C2DPolygon::Update()
{
	//�ړ��e�X�g
	if (INPUT->GetKeyPush(VK_RIGHT)==true)
	{
		m_Move.x = 1.0f;
	}
	if (INPUT->GetKeyPush(VK_LEFT) == true)
	{
		m_Move.x = -1.0f;
	}
	if (INPUT->GetKeyPush(VK_UP) == true)
	{
		m_Move.y = -1.0f;
	}
	if (INPUT->GetKeyPush(VK_DOWN) == true)
	{
		m_Move.y = 1.0f;
	}

	//��]�e�X�g
	if (INPUT->GetKeyPush(VK_RETURN) == true)
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
	m_Src=RECT_F(200.0f, 200.0f, 56.0f, 56.0f);
	
	//�`��ʒu
	m_Dst = RECT_F(m_ScreenPos.y, m_ScreenPos.x, 100.0f, 100.0f);
	
	//�`��
	DRAW->DrawTexture(0, &m_Src, &m_Dst, &m_Color, m_fRad);
}