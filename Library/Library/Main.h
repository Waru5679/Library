#pragma once

//�K�v�ȃw�b�_�[�t�@�C���̃C���N���[�h
#include <windows.h>
#include <D3D10.h>
#include <d3dx10.h>

//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")

//�E�C���h�E��
static WCHAR szAppName[] = L"���C�u����";

//�E�C���h�E�T�C�Y
constexpr int WINDOW_WIDTH{ 640 };
constexpr int WINDOW_HEIGHT{ 480 };

//�t���[�����[�g
constexpr int FRAME_RATE{ 60 };

//���߂̃V�[��
#define START_SCENE SceneName::SceneMain 

//�R�[���o�b�N�֐�
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

#define DX CDirectX::GetInstance()
#define SHADER CShader::GetInstance()
#define FRAME CFrameRate::GetInstance()
#define SCENE CSceneManager::GetInstance()

//�V�[��
enum SceneName
{
	SceneMain,	//���C��
};

//�I�u�W�F�N�g
enum ObjName
{
	ObjMainCamera,	//���C���J����
	ObjPlayer,		//�v���C���[
	ObjDraw2D,		//2D�`��
	ObjSphere,		//��
	ObjEnemy,		//�G
	ObjBullet,		//�e
};

//���f��
enum ModelName
{
	ModelPlayer,	//�v���C���[
	ModelSphere,	//��
};
