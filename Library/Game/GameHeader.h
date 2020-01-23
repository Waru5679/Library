#pragma once

//���߂̃V�[��
#define START_SCENE SceneName::SceneMain

//�Q�[���Ŏg���I�u�W�F�N�g�t�@�C��
#include "../Game/Main/2DPolygon.h"
#include "../Game/Main/MainCamera.h"
#include "../Game/Main/Enemy.h"
#include "../Game/Main/EnemyBullet.h"
#include "../Game/Main/Bullet.h"
#include "../Game/Main/Player.h"
#include "../Game/Main/Aiming.h"
#include "../Game/Title/TitleCamera.h"
#include "../Game/Title/TitleObj.h"

//�Q�[���Ŏg���V�[���t�@�C��
#include "../Game/Main/SceneMain.h"
#include "../Game/Title/SceneTitle.h"

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
	TexTitle,	//�^�C�g���e�N�X�`��
};

//�萔
constexpr int BULLET_DELETE_TIME{ 300 };	//�e�̍폜�^�C��