//���C�u�����t�@�C��
#include "LibraryHeader.h"

//�Q�[���Ŏg���V�[���t�@�C��
#include "../Game/GameHeader.h"

//�V�[���̍쐬
void CSceneInclude::LoadScene()
{
	//���C��
	CSceneMain* main = new CSceneMain();
	SCENE->Insert(main, SceneName::SceneMain);
}