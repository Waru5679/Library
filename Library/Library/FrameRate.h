#pragma once
#include "Main.h"

constexpr float MIN_FRAME_TIME{ 1000.0f / FRAME_RATE };

//�t���[�����[�g�v�Z�N���X
class CFrameRate
{
private:
	CFrameRate() {};	//�R���X�g���N�^
	~CFrameRate() {};	//�f�X�g���N�^

	//�C���X�^���X
	static CFrameRate* m_pInstance;

public:
	//�C���X�^���X�擾
	static CFrameRate* GetInstance()
	{
		//�C���X�^���X������ĂȂ���΃C���X�^���X��
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CFrameRate();
		}
		return m_pInstance;
	}

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