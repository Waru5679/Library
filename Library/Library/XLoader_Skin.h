#pragma once

#include "Main.h"
#include "Singleton.h"
#include "Struct.h"
#include "LibraryHeader.h"
#include "X_Struct.h"
#include <stdio.h>

//X�t�@�C���֘A�̃N���X
class CX_Skin : public CSingleton<CX_Skin>
{
private:
	//�V���O���g��
	friend CSingleton<CX_Skin>;
	CX_Skin() {};	//�R���X�g���N�^
	~CX_Skin() {};	//�f�X�g���N�^
public:
	void Release();//�J��

	//�X�L�����b�V���̓ǂݍ���
	bool LoadSkinMesh(const char* FileName, SKIN_MESH* pSkinMesh);

	//�A�j���[�V����
	void Animation(int AnimeId, int NowFrame, SKIN_MESH* pSkinMesh);

	//���b�V���`��(�e�X�g�p)
	void DrawMesh(D3DMATRIX matWorld, SKIN_MESH* pSkinMesh, CColorData* pColor);
private:
	
	//�{�[���̍X�V
	void BoneUpdate(SKIN_MESH* pSkin, int AnimeId, int NowFrame);

	//�|�[�Y���擾����
	D3DXMATRIX GetPose(bool* bFind, SKIN_MESH* pSkin, BONE* pBone, ANIMATION Anime, int NowFrame, int BoneID);

	//�E�F�C�g���傫�����Ƀ\�[�g����
	void WeightSort(SKIN_MESH* pSkin);

	//�t���[���⊮
	KEY FrameComplement(int NowFrame, ANIMATOIN_KEY AnimKey);
	SKIN_MESH m_SkinMesh;	//�X�L�����b�V��
};