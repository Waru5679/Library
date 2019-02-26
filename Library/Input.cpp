#include "Input.h"

CInput g_input;

void CInput::Init()
{
	m_mous_x = 0;
	m_mous_y = 0;

	for (int i = 0; i < 256; i++)
		m_key[i] = false;
}

//�}�E�X�ʒu�擾�p�@�E�B���h�E�v���W�[�W���[���ɐ錾
void CInput::SetMousPos(UINT* uMsg, LPARAM* lParam)
{
	switch (*uMsg)
	{
		case WM_MOUSEMOVE:	//�}�E�X���삵���Ƃ�
		{
			POINT point = { LOWORD(*lParam),HIWORD((*lParam)) };

			m_mous_x = point.x;	//�J�[�\����x���W
			m_mous_y = point.y;	//�J�[�\����y���W
		}
		break;
	}
}

//�L�[�E�}�E�X�{�^���̃v�b�V���m�F
//�{�^����������Ă����true,������Ă��Ȃ����false��Ԃ�
bool CInput::GetKeyPush(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		m_key[key] = true;
		return true;
	}
	m_key[key] = false;
	return false;
}

//�L�[�E�}�E�X�{�^���̃v�b�V���m�F (�g���K�[��)
//�V���������ꂽ�Ȃ�true,
//���łɉ�����Ă����@�܂��́@������Ă��Ȃ��Ȃ� false�@
bool CInput::GetTriggerKeyPush(int key)
{
	//�L�[��������Ă��Ă�ꍇ
	if ( (GetAsyncKeyState(key) & 0x8000) ) 
	{
		//�L�[�̃t���O���I�t�Ȃ�
		if (m_key[key] == false)
		{
			m_key[key] = true;
			return true;
		}
	}
	//�L�[��������Ă��Ȃ��ꍇ
	else
	{
		m_key[key] = false;
	}
	return false;
}