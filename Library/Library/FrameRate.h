#pragma once
#include <Windows.h>
#include "Main.h"

constexpr float MIN_FRAME_TIME{ 1.0f / FRAME_RATE };

//�t���[�����[�g�v�Z�N���X
class CFrameRate
{
public:
	void Init();	//������
	void Release();	//���

	void FrameCount();//�t���[���v�Z

private:
	void Draw();	//�`��

	DWORD m_NowTime;	//���ݎ���
	DWORD m_StartTime;	//�J�n����
	DWORD m_DrawTime;	//�`�掞��

	int m_Count;   //�t���[�����[�g
};

extern CFrameRate g_Frame;