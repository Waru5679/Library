#include "LibraryMain.h"
#include "Main.h"

//グローバル変数
HWND g_hWnd = NULL;

#include "SceneInclude.h"

#include "DirectX.h"
#include "Font.h"
#include "Audio.h"
#include "Shader.h"
#include "FrameRate.h"
#include "Hit.h"
#include "Scene.h"

//インスタンス
CLibraryMain* CLibraryMain::m_pInstance = nullptr;

//初期化
bool CLibraryMain::Init(HINSTANCE hInst)
{
	//window初期化
	WinInit(hInst);

	// DirectXの初期化関数を呼ぶ

	if (DX->Init(g_hWnd)==false)
	{
		return false;
	}

	//シェーダー初期化
	if (g_Shader.Init(DX->GetDevice())==false)
	{
		return false;
	}

	//描画の初期化
	g_Draw.Init();

	//フォント描画初期化
	g_Font.Init();

	//音楽初期化
	g_Audio.Init();

	//フレームレート
	g_Frame.Init();

	//当たり判定初期化
	g_Hit.Init();
	
	//シーンロード
	CSceneInclude::LoadScene();

	//スタートシーンセット
	g_Scene.SetScene(START_SCENE);
	
	return true;
}

//更新
void CLibraryMain::Update()
{
	//画面クリア (RGBA)
	float ClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	DX->GetDevice()->ClearRenderTargetView(DX->GetRenderTargetView(), ClearColor);

	//深度バッファクリア
	DX->GetDevice()->ClearDepthStencilView(DX->GetDepthStencilView(), D3D10_CLEAR_DEPTH, 1.0f, 0);

	//更新
	g_Scene.Update();

	//描画
	g_Scene.Draw();

	//当たり判定描画
	g_Hit.Draw();

	//フレームレート計算
	g_Frame.FrameCount();

	//フレームレート数描画
	g_Frame.Draw();

	//画面更新（バックバッファをフロントバッファに）
	DX->GetSwapChain()->Present(0, 0);
}

//ウインドウの初期設定
void CLibraryMain::WinInit(HINSTANCE hInst)
{
	// ウィンドウの設定
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

	//ウインドウ作成
	g_hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInst, NULL);

	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);
}

//メモリの開放
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

