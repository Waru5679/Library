#include "LibraryMain.h"


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
	//ウインドウハンドル
	m_hWnd = nullptr;

	//window初期化
	WinInit(hInst);

	// DirectXの初期化関数を呼ぶ
	if (DX->Init(m_hWnd)==false)
	{
		return false;
	}

	//シェーダー初期化
	if (SHADER->Init(DX->GetDevice())==false)
	{
		return false;
	}

	//描画の初期化
	DRAW->Init();

	//フォント描画初期化
	FONT->Init();

	//音楽初期化
	AUDIO->Init();

	//フレームレート
	FRAME->Init();

	//当たり判定初期化
	g_Hit.Init();
	
	//シーンロード
	CSceneInclude::LoadScene();

	//スタートシーンセット
	SCENE->SetScene(START_SCENE);
	
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
	SCENE->Update();

	//描画
	SCENE->Draw();

	//当たり判定描画
	g_Hit.Draw();

	//フレームレート計算
	FRAME->FrameCount();

	//フレームレート数描画
	FRAME->Draw();

	//画面更新（バックバッファをフロントバッファに）
	DX->GetSwapChain()->Present(0, 0);
}

//ウインドウの初期設定
void CLibraryMain::WinInit(HINSTANCE hInst)
{
	// ウィンドウの設定
	WNDCLASSEX wndclass;

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
	m_hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInst, NULL);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}

//メモリの開放
void  CLibraryMain::Release()
{
	g_Hit.Release();
	SCENE->Release();
	AUDIO->Release();
	FONT->Release();
	DRAW->Release();
	SHADER->Release();
	DX->Release();
	
	//インスタンス破棄
	delete m_pInstance;
	m_pInstance = nullptr;
}

