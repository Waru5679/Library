#pragma once

//���o�͂ɕK�v�ȃw�b�_�[
#include <Windows.h>

//���̓N���X
class CInput
{
public:
	void Init();//������
			
	//�}�E�X�ʒu�擾�@
	void SetMousPos(UINT* uMsg, LPARAM* lParam);

	//�L�[�E�}�E�X�{�^���̃v�b�V���m�F
	bool GetKeyPush(int key);			//�ʏ�
	bool GetTriggerKeyPush(int key);	//�g���K�[��

	//�}�E�X�̈ʒu���擾
	int GetMousPosX(){return m_mous_x;}
	int GetMousPosY(){return m_mous_y;}
private:
	int m_mous_x;	//�}�E�X�̈ʒuX
	int m_mous_y;	//�}�E�X�̈ʒuY
	bool m_key[256];//�L�[�̏�Ԃ�ۑ�����
};

extern CInput g_input;