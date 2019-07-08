#pragma once
#include "Main.h"
#include "Singleton.h"

constexpr float MIN_FRAME_TIME{ 1000.0f / FRAME_RATE };

//�t���[�����[�g�v�Z�N���X
class CFrameRate:public CSingleton<CFrameRate>
{
private:
	//�V���O���g�������ŃC���X�^���X��
	friend CSingleton<CFrameRate>;

	CFrameRate() {};	//�R���X�g���N�^
	~CFrameRate() {};	//�f�X�g���N�^
public:
	void Init();		//������
	void FrameCount();	//�t���[���v�Z
	void Draw();		//�`��
	void Release();		//���
private:
	DWORD m_NowTime;	//���ݎ���
	DWORD m_StartTime;	//�J�n����
	DWORD m_DrawTime;	//�`�掞��

	int m_FrameRate;	//�t���[�����[�g
	int m_Count;		//�J�E���^
		
	//�ϊ�
	wchar_t* m_pDrawStr;
};