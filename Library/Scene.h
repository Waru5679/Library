#pragma once

//�V�[���N���X(�p���p)
class CScene
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw()=0;
	virtual void Release()=0;

	
	int id;
private:
	virtual void LoadAudio() = 0;		//���y
	virtual void LoadTexture() = 0;		//�e�N�X�`��
	virtual void LoadMesh() = 0;		//���b�V��
};

//�V�[���}�l�[�W���[
class CSceneManager
{
public:
	void SetScene(int n);
	void Update();
	void Draw();
private:
	CScene* m_pNowScene;
};

extern CSceneManager g_Scene;



