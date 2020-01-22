//���C�u�����t�@�C��
#include "../Library/LibraryHeader.h"

//�Q�[���t�@�C��
#include "GameHeader.h"

//������
void CTitleObj::Init()
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
	m_Color = CColorData(1.0f, 1.0f, 1.0f, 0.5f);
}

//�X�V
void CTitleObj::Update()
{
	//�ړ��e�X�g
	if (INPUT->GetKeyPush(VK_RETURN) == true)
	{
		SCENE->SetScene(SceneName::SceneMain);
	}
	
}

//�`��
void CTitleObj::Draw()
{
	//�؂���ʒu
	m_Src = RECT_F(200.0f, 200.0f, 56.0f, 56.0f);

	//�`��ʒu
	m_Dst = RECT_F(m_ScreenPos.y, m_ScreenPos.x, 100.0f, 100.0f);

	//�`��
	DRAW->DrawTexture(TexName::TexTest, &m_Src, &m_Dst, &m_Color, m_fRad);
}
