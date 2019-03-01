#pragma once
#include <d3dx10.h>
#include <D3D10.h>
#include <vector>
#include <windef.h>

#include "CObj.h"
#include "Scene.h"
#include "Camera.h"
using namespace std;

//�Ǘ��p�@�N���X
class CTask
{
public:
	void Update();	//�X�V
	void Draw();	//�`��
	void Release();	//���
		
	//�o�^
	void InsertObj(CObj* pObj, int Id);
	void Insert(MY_MESH Mesh, int Id);
	void InsertScene(CScene* Scene, int Id);
	void Insert(ID3D10ShaderResourceView* pTex, int Id);
	
	//�擾	
	CObj* GetObj(int Id);
	MY_MESH GetMesh(int Id);
	CScene* GetScene(int Id);
	ID3D10ShaderResourceView* GetTex(int Id);
	
	//OBJ���X�g�f�[�^�폜
	void ObjDelete();
private:
	vector<CScene*> m_Scene;					//�V�[��
	vector<MY_MESH> m_Mesh;						//���b�V��
	vector<CObj*> m_Obj;						//�I�u�W�F�N�g
	vector<ID3D10ShaderResourceView*> m_Tex;	//�e�N�X�`��
};

extern CTask g_Task;
