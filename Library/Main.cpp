//必要なヘッダーファイルのインクルード
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

//グローバル変数
HWND g_hWnd=NULL;

//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

//ウインドウの初期設定
void WinInit(HINSTANCE hInst)
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

//アプリケーションのエントリー関数
INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR szStr,INT iCmdShow)
{
	//ウインドウの初期設定
	WinInit(hInst);
		
	// DirectXの初期化関数を呼ぶ
	if(FAILED(dx.Init(g_hWnd)))
	{
		return 0;
	}

	//シェーダー初期化
	if (FAILED(g_Shader.Init(dx.m_pDevice)))
	{
		return E_FAIL;
	}

	//描画の初期化
	g_Draw.Init();

	//フォント描画初期化
	CFont::Init();

	//音楽初期化
	g_Audio.Init();
	
	// メッセージループ
	MSG msg;
    ZeroMemory( &msg, sizeof(msg) );
		
	//シーンロード
	LoadScene();

	//スタートシーンセット
	g_Scene.SetScene(START_SCENE);
	
	//fps制御
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
			//画面クリア (RGBA)
			float ClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
			dx.m_pDevice->ClearRenderTargetView(dx.m_pRenderTargetView,ClearColor);

			//深度バッファクリア
			dx.m_pDevice->ClearDepthStencilView(dx.m_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);

			nowTime = GetTickCount();

			//60fps
			if (nowTime - startTime > (int)1000 / 60)
			{
				startTime = nowTime;

				//更新
				g_Scene.Update();
			}

			//描画
			g_Scene.Draw();

			//画面更新（バックバッファをフロントバッファに）
			dx.m_pSwapChain->Present(0, 0);
		}				
	}

	//メモリ解放
	g_Audio.Release();	
	CFont::Release();
	g_Draw.Release();
	g_Shader.Release();
	dx.Release();		
	
	return (INT)msg.wParam;
}

// ウィンドウプロシージャー関数
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

//シーン読み込み
void LoadScene()
{
	//メイン
	CSceneMain* main = new CSceneMain();
	g_Task.InsertScene(main, Scene_Main);

}

