#pragma once

//シーンクラス(継承用)
class CScene
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw()=0;
	virtual void Release()=0;

	
	int id;
private:
	virtual void LoadAudio() = 0;		//音楽
	virtual void LoadTexture() = 0;		//テクスチャ
	virtual void LoadMesh() = 0;		//メッシュ
};

//シーンマネージャー
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



