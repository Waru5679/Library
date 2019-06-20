#pragma once
#include <Windows.h>
#include "Main.h"

constexpr float MIN_FRAME_TIME{ 1000.0f / FRAME_RATE };

//�t���[�����[�g�v�Z�N���X
class CFrameRate
{
public:
	void Init();		//������
	void FrameCount();	//�t���[���v�Z
	void Draw();		//�`��

	int GetCount() { return m_Count; }
private:
	DWORD m_NowTime;	//���ݎ���
	DWORD m_StartTime;	//�J�n����
	DWORD m_DrawTime;	//�`�掞��

	int m_FrameRate;	//�t���[�����[�g
	int m_Count;		//�J�E���^
};

extern CFrameRate g_Frame;