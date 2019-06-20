#include "FrameRate.h"
#include "Font.h"

CFrameRate g_Frame;

//������
void CFrameRate::Init()
{
	//���x���グ��
	timeBeginPeriod(1);

	//�J�n�����擾
	m_StartTime = timeGetTime();
	
	//�`�掞��
	m_DrawTime = m_StartTime;
		
	//�J�E���^
	m_Count = 0;
}

//�t���[�����[�g�̌v�Z
void CFrameRate::FrameCount()
{
	//�J�E���^�X�V
	m_Count++;

	//���ݎ����擾
	m_NowTime = timeGetTime();

	//���̃t���[���ł�����������
	float FrameTime = static_cast<float>(m_NowTime - m_StartTime)/1000;

	//���Ԃɗ]�T������Ƃ�
	if ( FrameTime < MIN_FRAME_TIME)
	{
		//�]���Ȏ���
		DWORD SleepTime = static_cast<DWORD>((MIN_FRAME_TIME - FrameTime)* 1000);
		
		//�ҋ@
		Sleep(SleepTime);
	}

	//1�b�o�߂��Ă�Ƃ�
	float draw_time = static_cast<float>(m_NowTime - m_DrawTime) / 1000;
	if ( draw_time >= 1.0f)
	{
		//�`�掞���X�V
		m_DrawTime = m_NowTime;

		//�t���[�����`��
		Draw();

		//�J�E���^������
		m_Count = 0;
	}

	//�J�n�����X�V
	m_StartTime = m_NowTime;
}

//�`��
void CFrameRate::Draw()
{
	//�ϊ�
	wchar_t buf[12];
	_itow_s(m_Count, buf, 10);

	g_Font.DrawStr(buf, 0.0f, 0.0f, 32.0f, 0.0f);
}

//���
void CFrameRate::Release()
{
	//���x��߂�
	timeEndPeriod(1);
}