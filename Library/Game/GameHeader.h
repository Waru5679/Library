#pragma once

//���߂̃V�[��
#define START_SCENE SceneName::SceneTitle

//�Q�[���Ŏg���I�u�W�F�N�g�t�@�C��
#include "../Game/2DPolygon.h"
#include "../Game/Street.h"
#include "../Game/MainCamera.h"
#include "../Game/Enemy.h"
#include "../Game/EnemyBullet.h"
#include "../Game/Bullet.h"
#include "../Game/Player.h"
#include "../Game/Aiming.h"
#include "../Game/TitleCamera.h"
#include "../Game/TitleObj.h"

//Debug
#include "../Game/RayTest.h"
#include "../Game/Sphere.h"

//�Q�[���Ŏg���V�[���t�@�C��
#include "../Game/SceneMain.h"
#include "../Game/SceneTitle.h"

//�V�[��
enum SceneName
{
	SceneMain,	//���C��
	SceneTitle,	//�^�C�g��
};

//�I�u�W�F�N�gID(������)
enum ObjName
{
	ObjPlayer,		//�v���C���[
	ObjMainCamera,	//���C���J����
	ObjDraw2D,		//2D�`��
	ObjSphere,		//��
	ObjEnemy,		//�G
	ObjEnemyBullet,	//�G�̒e
	ObjBullet,		//�e
	ObjAiming,		//�Ə�
	ObjTest,		//�e�X�g
	ObjVmdTest,		//vmd
	ObjTitleCamera,	//�^�C�g���J����
	ObjTitle,		//�^�C�g��Obj
};

//���f��
enum ModelName
{
	ModelPlayer,	//�v���C���[
	ModelSphere,	//��
	ModelAiming,	//�Ə�
	ModelTarget,	//�I
};

//�e�N�X�`��
enum TexName
{
	TexTest	,	//�`��e�X�g
};

//�萔
constexpr int BULLET_DELETE_TIME{ 300 };	//�e�̍폜�^�C��