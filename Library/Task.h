#pragma once
#include <d3dx10.h>
#include <D3D10.h>
#include <vector>
#include <windef.h>

#include "CObj.h"
#include "Scene.h"
#include "Camera.h"
using namespace std;

//�e�N�X�`���Ǘ��p
struct MY_TEXTURE
{
	ID3D10ShaderResourceView* m_pTex;	//�e�N�X�`��
	int m_Id;							//�Ǘ��pID

};

//�V�[���Ǘ��p
struct MY_SCENE
{
	CScene* m_pScene;	//�V�[��
	int m_Id;			//�Ǘ��pID
};

//�Ǘ��p�@�N���X
class CTask
{
public:
	void Update();	//�X�V
	void Draw();	//�`��
	void Release();	//���
		
	//�o�^
	void InsertObj(CObj* pObj, int Id);
	void InsertScene(CScene* pScene, int Id);
	void Insert(ID3D10ShaderResourceView* pTex, int Id);
	
	//�擾	
	CObj* GetObj(int Id);
	CScene* GetScene(int Id);
	ID3D10ShaderResourceView* GetTex(int Id);
	
	//OBJ���X�g�f�[�^�폜
	void ObjDelete();
private:
	vector<MY_SCENE> m_Scene;	//�V�[��
	vector<CObj*> m_Obj;		//�I�u�W�F�N�g
	vector<MY_TEXTURE> m_Tex;	//�e�N�X�`��
};

extern CTask g_Task;
