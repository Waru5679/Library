#pragma once
#include "Main.h"
#include "Singleton.h"

//���C�u�������C���N���X
class CLibraryMain:public CSingleton<CLibraryMain>
{
private:
	//�V���O���g���N���X�����ŃC���X�^���X������
	friend CSingleton<CLibraryMain>;	

	CLibraryMain() {};	//�R���X�g���N�^
	~CLibraryMain() {};	//�f�X�g���N�^
public:
	//������
	bool Init(HINSTANCE hInst);

	//�X�V
	void Update();

	//�������̊J��
	void Release();	
private:	
	//window�����ݒ�
	void WinInit(HINSTANCE hInst);

	//�E�C���h�E�n���h��
	HWND m_hWnd;
};