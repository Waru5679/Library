//���C�u�����t�@�C��
#include "SceneInclude.h"
#include "Task.h"

//�Q�[���Ŏg���V�[���t�@�C��
#include "../Game/SceneMain.h"

//�V�[���̍쐬
void CSceneInclude::LoadScene()
{
	//���C��
	CSceneMain* main = new CSceneMain();
	g_Task.InsertScene(main, SceneName::SceneMain);

}