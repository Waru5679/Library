#pragma once

//�V�[���N���X(�p���p)
class CScene
{
public:
	virtual void Init() = 0;	//������
	virtual void Update() = 0;	//�X�V
	virtual void Draw()=0;		//�`��
	virtual void Release()=0;	//���

	int m_id; //�V�[��ID
private:
	virtual void LoadAudio() = 0;		//���y
	virtual void LoadTexture() = 0;		//�e�N�X�`��
	virtual void LoadMesh() = 0;		//���b�V��
};

//�V�[���}�l�[�W���[
class CSceneManager
{
public:
	void SetScene(int n);	//�V�[���̃Z�b�g
	void Update();			//�X�V
	void Draw();			//�`��
private:
	CScene* m_pNowScene;	//���݂̃V�[���̃|�C���^
};

extern CSceneManager g_Scene;



