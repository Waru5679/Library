//�K�v�ȃw�b�_�[�t�@�C���̃C���N���[�h
#include <stdio.h>
#include <vector>

#include "DirectX.h"
#include "Task.h"
#include "Main.h"
#include "Shader.h"
#include "Polygon.h"
#include "Font.h"
#include "Audio.h"

#include "SceneMain.h"

using namespace std;

//�O���[�o���ϐ�
HWND g_hWnd=NULL;

//�֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

//�E�C���h�E�̏����ݒ�
void WinInit(HINSTANCE hInst)
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

//�A�v���P�[�V�����̃G���g���[�֐�
INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR szStr,INT iCmdShow)
{
	//�E�C���h�E�̏����ݒ�
	WinInit(hInst);
		
	// DirectX�̏������֐����Ă�
	if(FAILED(dx.Init(g_hWnd)))
	{
		return 0;
	}

	//�V�F�[�_�[������
	if (FAILED(g_Shader.Init(dx.m_pDevice)))
	{
		return E_FAIL;
	}

	//�`��̏�����
	g_Draw.Init();

	//�t�H���g�`�揉����
	CFont::Init();

	//���y������
	g_Audio.Init();
	
	// ���b�Z�[�W���[�v
	MSG msg;
    ZeroMemory( &msg, sizeof(msg) );
		
	//�V�[�����[�h
	LoadScene();

	//�X�^�[�g�V�[���Z�b�g
	g_Scene.SetScene(START_SCENE);
	
	//fps����
	DWORD startTime = GetTickCount();
	DWORD nowTime = 0;
	
	while( msg.message!=WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
	    {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
	    }
	    else
		{
			//��ʃN���A (RGBA)
			float ClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
			dx.m_pDevice->ClearRenderTargetView(dx.m_pRenderTargetView,ClearColor);

			//�[�x�o�b�t�@�N���A
			dx.m_pDevice->ClearDepthStencilView(dx.m_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);

			nowTime = GetTickCount();

			//60fps
			if (nowTime - startTime > (int)1000 / 60)
			{
				startTime = nowTime;

				//�X�V
				g_Scene.Update();
			}

			//�`��
			g_Scene.Draw();

			//��ʍX�V�i�o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɂj
			dx.m_pSwapChain->Present(0, 0);
		}				
	}

	//���������
	g_Audio.Release();	
	CFont::Release();
	g_Draw.Release();
	g_Shader.Release();
	dx.Release();		
	
	return (INT)msg.wParam;
}

// �E�B���h�E�v���V�[�W���[�֐�
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{	
	switch(iMsg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_ESCAPE:

				PostQuitMessage(0);
				break;
			}
		}
		break;		
	}
	return DefWindowProc (hWnd, iMsg, wParam, lParam);	
}

//�V�[���ǂݍ���
void LoadScene()
{
	//���C��
	CSceneMain* main = new CSceneMain();
	g_Task.InsertScene(main, Scene_Main);

}

