#pragma once

//シーンクラス(継承用)
class CScene
{
public:
	virtual void Init() = 0;	//初期化
	virtual void Update() = 0;	//更新
	virtual void Draw()=0;		//描画
	virtual void Release()=0;	//解放

	int m_id; //シーンID
private:
	virtual void LoadAudio() = 0;		//音楽
	virtual void LoadTexture() = 0;		//テクスチャ
	virtual void LoadMesh() = 0;		//メッシュ
};

//シーンマネージャー
class CSceneManager
{
public:
	void SetScene(int n);	//シーンのセット
	void Update();			//更新
	void Draw();			//描画
private:
	CScene* m_pNowScene;	//現在のシーンのポインタ
};

extern CSceneManager g_Scene;



