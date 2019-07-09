#pragma once

//���C�u�����Ŏg�p���鎩��w�b�_�t�@�C��
#include "Singleton.h"
#include "SceneInclude.h"
#include "DirectX.h"
#include "Font.h"
#include "Audio.h"
#include "Shader.h"
#include "FrameRate.h"
#include "Hit.h"
#include "Scene.h"
#include "Hit.h"
#include "Release.h"
#include "ObjLoader.h"
#include "Input.h"
#include "Ray.h"
#include "Polygon.h"

//�ς��邩������Ȃ����ǂ����ŃC���N���[�h�������
#include "Struct.h"
#include "Math.h"
#include "Task.h"


//���C�u�����Ŏg�p����萔�}�N��
#define DX CDirectX::GetInstance()
#define SHADER CShader::GetInstance()
#define FRAME CFrameRate::GetInstance()
#define SCENE CSceneManager::GetInstance()
#define AUDIO CAudio::GetInstance()
#define FONT CFont::GetInstance()
#define DRAW CDraw::GetInstance()
#define HIT CHit::GetInstance()
#define LOADER CObjLoader::GetInstance()
#define INPUT CInput::GetInstance()
#define RAY CRay::GetInstance()