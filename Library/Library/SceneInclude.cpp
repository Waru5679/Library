//ライブラリファイル
#include "LibraryHeader.h"

//ゲームで使うシーンファイル
#include "GameHeader.h"

//シーンの作成
void CSceneInclude::LoadScene()
{
	//メイン
	CSceneMain* main = new CSceneMain();
	SCENE->Insert(main, SceneName::SceneMain);
}