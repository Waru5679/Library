#include "LibraryMain.h"
#include "Main.h"

//�O���[�o���ϐ�
HWND g_hWnd = NULL;

#include "SceneInclude.h"

#include "DirectX.h"
#include "Font.h"
#include "Audio.h"
#include "Shader.h"
#include "FrameRate.h"
#include "Hit.h"
#include "Scene.h"

//�C���X�^���X
CLibraryMain* CLibraryMain::m_pInstance = nullptr;

//������
bool CLibraryMain::Init(HINSTANCE hInst)
{
	//window������
	WinInit(hInst);

	// DirectX�̏������֐����Ă�

	if (DX->Init(g_hWnd)==false)
	{
		return false;
	}

	//�V�F�[�_�[������
	if (g_Shader.Init(DX->GetDevice())==false)
	{
		return false;
	}

	//�`��̏�����
	g_Draw.Init();

	//�t�H���g�`�揉����
	g_Font.Init();

	//���y������
	g_Audio.Init();

	//�t���[�����[�g
	g_Frame.Init();

	//�����蔻�菉����
	g_Hit.Init();
	
	//�V�[�����[�h
	CSceneInclude::LoadScene();

	//�X�^�[�g�V�[���Z�b�g
	g_Scene.SetScene(START_SCENE);
	
	return true;
}

//�X�V
void CLibraryMain::Update()
{
	//��ʃN���A (RGBA)
	float ClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	DX->GetDevice()->ClearRenderTargetView(DX->GetRenderTargetView(), ClearColor);

	//�[�x�o�b�t�@�N���A
	DX->GetDevice()->ClearDepthStencilView(DX->GetDepthStencilView(), D3D10_CLEAR_DEPTH, 1.0f, 0);

	//�X�V
	g_Scene.Update();

	//�`��
	g_Scene.Draw();

	//�����蔻��`��
	g_Hit.Draw();

	//�t���[�����[�g�v�Z
	g_Frame.FrameCount();

	//�t���[�����[�g���`��
	g_Frame.Draw();

	//��ʍX�V�i�o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɂj
	DX->GetSwapChain()->Present(0, 0);
}

//�E�C���h�E�̏����ݒ�
void CLibraryMain::WinInit(HINSTANCE hInst)
{
	// �E�B���h�E�̐ݒ�
	WNDCLASSEX  wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_ASTERISK);
	RegisterClassEx(&wndclass);

	//�E�C���h�E�쐬
	g_hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInst, NULL);

	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);
}

//�������̊J��
void  CLibraryMain::Release()
{
	g_Hit.Release();
	g_Scene.Release();
	g_Audio.Release();
	g_Font.Release();
	g_Draw.Release();
	g_Shader.Release();
	DX->Release();
}

