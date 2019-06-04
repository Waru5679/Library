#pragma once

//�K�v�ȃw�b�_�[�t�@�C���̃C���N���[�h
#include <windows.h>
#include <D3D10.h>
#include <d3dx10.h>

//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")

//�E�C���h�E�T�C�Y
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

//�������̊J��
void Release();

//�E�C���h�E��
static WCHAR szAppName[] = L"���C�u����";

//���߂̃V�[��
#define START_SCENE SceneName::SceneMain

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
	ObjStreet,		//��
	ObjRayTest,	//Ray�̃e�X�g
	ObjSphere,	//��
};

//���f��
enum ModelName
{
	ModelStreet,	//��
	ModelPlayer,	//�v���C���[

	ModelRayTest,	//Ray�e�X�g
	ModelSphere,	//��
};
