#pragma once
#include <vector>
using namespace std;

//シーンクラス(継承用)
class CSceneBase
{
public:
	virtual void Init() = 0;	//初期化
	virtual void Update() = 0;	//更新
	virtual void Draw()=0;		//描画
	virtual void Release()=0;	//解放
private:
	virtual void LoadAudio() = 0;		//音楽
	virtual void LoadTexture() = 0;		//テクスチャ
	virtual void LoadMesh() = 0;		//メッシュ
};

//シーン管理用構造体
struct MY_SCENE
{
	CSceneBase* m_pScene;	//シーンポインタ
	int m_Id;				//管理用ID
};

//シーンマネージャー
class CSceneManager
{
public:
	void Update();							//更新
	void Draw();							//描画
	void Release();							//解放
	void Insert(CSceneBase* pScene, int Id);//登録
	void SetScene(int n);					//シーンのセット
	CSceneBase* GetScene(int Id);			//シーン取得
private:
	CSceneBase* m_pNowScene;				//現在のシーンのポインタ
	vector<MY_SCENE> m_Scene;				//シーンリスト
};

extern CSceneManager g_Scene;



