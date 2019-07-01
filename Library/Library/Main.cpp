//必要なヘッダーファイルのインクルード
#include "Main.h"
#include "LibraryMain.h"

//インスタンス
#define LIB_MAIN CLibraryMain::GetInstance()

//アプリケーションのエントリー関数
INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR szStr,INT iCmdShow)
{
	//初期化
	LIB_MAIN->Init(hInst);

	// メッセージループ
	MSG msg;
    ZeroMemory( &msg, sizeof(msg) );

	while( msg.message!=WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
	    {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
	    }
	    else
		{
			//更新
			LIB_MAIN->Update();
		}				
	}
	return (INT)msg.wParam;
}

// ウィンドウプロシージャー関数
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{	
	switch(iMsg)
	{
		//x印
		case WM_DESTROY:
		{
			LIB_MAIN->Release();	//メモリ解放

			PostQuitMessage(0);
			break;
		}
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				//ESC
				case VK_ESCAPE:
				{
					LIB_MAIN->Release();	//メモリ解放

					PostQuitMessage(0);
					break;
				}
			}
		}
		break;		
	}
	return DefWindowProc (hWnd, iMsg, wParam, lParam);	
}
