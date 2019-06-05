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
	g_Scene.Insert(main, SceneName::SceneMain);
}