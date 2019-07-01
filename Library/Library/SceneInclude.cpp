//ライブラリファイル
#include "SceneInclude.h"
#include "Task.h"

//ゲームで使うシーンファイル
#include "../Game/SceneMain.h"

//シーンの作成
void CSceneInclude::LoadScene()
{
	//メイン
	CSceneMain* main = new CSceneMain();
	SCENE->Insert(main, SceneName::SceneMain);
}