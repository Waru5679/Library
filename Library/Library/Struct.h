#pragma once

//RECT�\����
struct RECT_F
{
	//�R���X�g���N�^
	RECT_F() {}
	RECT_F(float Top, float Left, float Width, float Height)
	{
		m_Top = Top;
		m_Left = Left;
		m_Right = Left + Width;
		m_Bottom = Top + Height;
	}

	float m_Top;	//��
	float m_Left;	//��
	float m_Right;	//�E
	float m_Bottom;	//��
};

//�F�f�[�^
struct ColorData
{
	//�R���X�g���N�^
	ColorData() {}
	ColorData(float R, float G, float B, float A)
	{
		m_Red = R;
		m_Green = G;
		m_Blue = B;
		m_Alpha = A;
	}
	
	float m_Red;	//��
	float m_Green;	//��
	float m_Blue;	//��
	float m_Alpha;	//����
};