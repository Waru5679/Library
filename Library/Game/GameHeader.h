#pragma once

//���߂̃V�[��
#define START_SCENE SceneName::SceneMain 

//�Q�[���Ŏg���I�u�W�F�N�g�t�@�C��
#include "../Game/2DPolygon.h"
#include "../Game/Street.h"
#include "../Game/MainCamera.h"
#include "../Game/Enemy.h"
#include "../Game/EnemyBullet.h"
#include "../Game/Bullet.h"
#include "../Game/Player.h"
#include "../Game/Aiming.h"

//Debug
#include "../Game/RayTest.h"
#include "../Game/Sphere.h"

//�Q�[���Ŏg���V�[���t�@�C��
#include "../Game/SceneMain.h"

//�V�[��
enum SceneName
{
	SceneMain,	//���C��
};

//�I�u�W�F�N�g
enum ObjName
{
	ObjPlayer,		//�v���C���[
	ObjDraw2D,		//2D�`��
	ObjSphere,		//��
	ObjEnemy,		//�G
	ObjEnemyBullet,	//�G�̒e
	ObjBullet,		//�e
	ObjAiming,		//�Ə�
};

//���f��
enum ModelName
{
	ModelPlayer,	//�v���C���[
	ModelSphere,	//��
	ModelAiming,	//�Ə�
};

//�e�N�X�`��
enum TexName
{
	TexTest	,	//�`��e�X�g
};