#pragma once

#include <Windows.h>
#include "Singleton.h"

constexpr int KEY_BOARD_ARRAY_SIZE{ 256 };

//���̓N���X
class CInput:public CSingleton<CInput>
{
public:
	void Init();	//������
	void Release();	//���
			
	//�}�E�X�ʒu�擾�@
	void SetMousPos(UINT* uMsg, LPARAM* lParam);

	//�L�[�E�}�E�X�{�^���̃v�b�V���m�F
	bool GetKeyPush(int key);			//�ʏ�
	bool GetTriggerKeyPush(int key);	//�g���K�[��

	//�}�E�X�̈ʒu���擾
	int GetMousPosX(){return m_Mous_x;}
	int GetMousPosY(){return m_Mous_y;}
private:
	int m_Mous_x;	//�}�E�X�̈ʒuX
	int m_Mous_y;	//�}�E�X�̈ʒuY

	bool m_Key[KEY_BOARD_ARRAY_SIZE];//�L�[�̏�Ԃ�ۑ�����
};