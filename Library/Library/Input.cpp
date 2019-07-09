#include "Input.h"
#include "Release.h"

//�C���X�^���X
CInput* CInput::m_pInstance = nullptr;

//������
void CInput::Init()
{
	m_Mous_x = 0;
	m_Mous_y = 0;

	//�L�[��񏉊���
	for (int i = 0; i < KEY_BOARD_ARRAY_SIZE; i++)
		m_Key[i] = false;
}

//�}�E�X�ʒu�擾
void CInput::SetMousPos(UINT* uMsg, LPARAM* lParam)
{
	switch (*uMsg)
	{
		case WM_MOUSEMOVE:	//�}�E�X���삵���Ƃ�
		{
			POINT point = { LOWORD(*lParam),HIWORD((*lParam)) };

			m_Mous_x = point.x;	//�J�[�\����x���W
			m_Mous_y = point.y;	//�J�[�\����y���W
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
		m_Key[key] = true;
		return true;
	}
	m_Key[key] = false;
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
		if (m_Key[key] == false)
		{
			m_Key[key] = true;
			return true;
		}
	}
	//�L�[��������Ă��Ȃ��ꍇ
	else
	{
		m_Key[key] = false;
	}
	return false;
}

//���
void CInput::Release()
{
	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}