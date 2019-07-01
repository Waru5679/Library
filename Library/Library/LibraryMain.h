#pragma once

#include "Main.h"

//�E�C���h�E��
static WCHAR szAppName[] = L"���C�u����";

//���C�u�������C���N���X
class CLibraryMain
{
private:
	CLibraryMain() {};	//�R���X�g���N�^
	~CLibraryMain() {};	//�f�X�g���N�^

	//�C���X�^���X
	static CLibraryMain* m_pInstance;

public:
	//�C���X�^���X�擾
	static CLibraryMain* GetInstance()
	{
		//�C���X�^���X������ĂȂ���΃C���X�^���X��
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CLibraryMain();
		}

		return m_pInstance;
	}

	//������
	bool Init(HINSTANCE hInst);

	//�X�V
	void Update();

	//�������̊J��
	void Release();	
private:	
	//window�����ݒ�
	void WinInit(HINSTANCE hInst);
};
