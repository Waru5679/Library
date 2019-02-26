#pragma once
#include <d3dx10.h>
#include <D3D10.h>
#include <vector>
#include <windef.h>

#include "CObj.h"
#include "Scene.h"
#include "Camera.h"
using namespace std;

class CTask
{
public:
	CTask() {};
	~CTask() {};

	//ìoò^
	void Insert3DObj(CObj3D* Obj, int Id);
	void Insert(MY_MESH Mesh, int Id);
	void InsertScene(CScene* Scene, int Id);
	void Insert(ID3D10ShaderResourceView* pTex, int Id);
	void InsertCamera(Camera* pCamera, int Id);
	void Insert2DObj(CObj2D* Obj, int Id);

	//éÊìæä÷êî
	CObj3D* Get3DObj(int Id);
	CObj2D* Get2DObj(int Id);

	MY_MESH GetMesh(int Id);
	CScene* GetScene(int Id);
	ID3D10ShaderResourceView* GetTex(int Id);
	Camera* GetCamera(int Id);

	//ÉfÅ[É^çÌèú
	void ObjDelete();

	void Update();
	void Draw();
	void Release();
	
private:
	vector<CScene*> m_Scene;
	vector<CObj3D*> m_3DObj;
	vector<CObj2D*> m_2DObj;
	vector<MY_MESH> m_Mesh;
	vector<ID3D10ShaderResourceView*> m_Tex;

	vector<Camera *> m_Camera;

};

extern CTask g_Task;
