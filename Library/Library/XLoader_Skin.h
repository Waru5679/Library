#pragma once

#include "Main.h"
#include "Singleton.h"
#include "Struct.h"
#include "LibraryHeader.h"
#include <stdio.h>

constexpr int TRIANGLE_POLYGON{ 3 };	//�O�p�|���S��
constexpr int QUAD_POLYGON{ 4 };		//�l�p�|���S��
constexpr int READ_ARRAY_SIZE{ 200 };	//�ǂݍ��ݗp�L�[�z��̃T�C�Y
constexpr int NAME_ARRAY_SIZE{ 100 };	//���O�z��̃T�C�Y

constexpr int MAX_VER_WEIGH{ 4 };		//���_�̍ő�E�F�C�g

//���_�\����
struct VERTEX
{
	VERTEX()
	{
		for (int i = 0; i < MAX_VER_WEIGH; i++)
		{
			m_BoneIndex[i] = -1;
			m_fWeight[i] = 0.0f;
		}
		m_WeightNum = 0;
	}

	D3DXVECTOR3 m_vPos;	//���_�ʒu
	D3DXVECTOR3 m_vNorm;//���_�@��
	D3DXVECTOR2 m_vTex;	//UV���W

	int m_BoneIndex[MAX_VER_WEIGH];	//�e�����󂯂�{�[���̃C���f�b�N�X
	float m_fWeight[MAX_VER_WEIGH];	//�E�F�C�g
	int m_WeightNum;				//�E�F�C�g��
};

//�|���S��
struct FACE
{
	FACE()
	{
		m_FaceOfVer = -1;
		m_pIndex = nullptr;
		m_UseMaterial = -1;
	}
	int m_FaceOfVer;	//�\�����钸�_�̐�
	int* m_pIndex;		//�\�����钸�_�̃C���f�b�N�X
	int m_UseMaterial;	//�g�p����}�e���A��
};

//�}�e���A���\����
struct MATERIAL
{
	MATERIAL()
	{
		ZeroMemory(this, sizeof(MATERIAL));
		m_pTexture = nullptr;
		m_pFaceIndex = nullptr;
		m_ppIndexBuffer = nullptr;
	}

	char						m_TexName[NAME_ARRAY_SIZE];	//�t�@�C����
	D3DXVECTOR4					m_vFaceColor;				//�ʂ̐F
	D3DXVECTOR3					m_vKs;						//�X�y�L�����[
	float						m_fPower;					//�X�y�L�����[�̃p���[
	D3DXVECTOR3					m_vKe;						//�G�~�b�V�u
	ID3D10ShaderResourceView* m_pTexture;					//�e�N�X�`���|�C���^	
	int							m_FaceNum;					//���̃}�e���A�����g�p����ʂ̐�
	int* m_pFaceIndex;				//���̃}�e���A���Ŏg�p����ʂ̃C���f�b�N�X���X�g
	ID3D10Buffer** m_ppIndexBuffer;			//�C���f�b�N�X�o�b�t�@
};

//�{�[���\����
struct BONE
{
	BONE()
	{
		m_pChildIndex = nullptr;
		D3DXMatrixIdentity(&m_matBindPose);
		D3DXMatrixIdentity(&m_matNewPose);
		D3DXMatrixIdentity(&m_matOffset);
	}
	char		m_Name[NAME_ARRAY_SIZE];	//�{�[����
	int			m_index;					//���g�̃C���f�b�N�X
	int			m_ChildNum;					//�q�̐�
	int* m_pChildIndex;				//�����̎q�̃C���f�b�N�X���X�g
	D3DXMATRIX	m_matBindPose;				//�����|�[�Y�i�����ƕς��Ȃ��j
	D3DXMATRIX	m_matNewPose;				//���݂̃|�[�Y�i���̓s�x�ς��j
	D3DXMATRIX  m_matOffset;				//�I�t�Z�b�g�s��
};

//�ǂ̃{�[�����x�̒��_�ɂǂꂾ���̉e����^���邩
struct SKIN_WEIGHT
{
	SKIN_WEIGHT()
	{
		m_WeightNum = -1;
		m_pIndex = nullptr;
		m_pWeight = nullptr;
		D3DXMatrixIdentity(&m_matOffset);
	}

	char		m_BoneName[NAME_ARRAY_SIZE];//�{�[����
	int			m_WeightNum;				//�E�F�C�g�̐�
	int* m_pIndex;					//�e�����󂯂钸�_�̃C���f�b�N�X���X�g
	float* m_pWeight;					//�E�F�C�g���X�g
	D3DXMATRIX	m_matOffset;				//�I�t�Z�b�g�s��
};

//�A�j���[�V�����̃L�[
struct KEY
{
	KEY()
	{
		m_Time = -1;
		m_ValueNum = -1;
		m_pfValue = nullptr;
	}

	int		m_Time;		//�R�}
	int		m_ValueNum;	//�l�̐�
	float* m_pfValue;	//�l�̃��X�g
};

//�{�[���̃L�[
struct BONE_KEY
{
	BONE_KEY()
	{
		m_KeyNum = -1;
		m_KeyType = -1;
		m_pKey = nullptr;
	}

	char	m_AffectBoneName[NAME_ARRAY_SIZE];	//�e�����󂯂�{�[����
	int		m_KeyType;							//�L�[�^�C�v
	int		m_KeyNum;							//�L�[�̐�
	KEY* m_pKey;								//�L�[���X�g
};

//�A�j���[�V�����\����
struct ANIMATION
{
	ANIMATION()
	{
		m_BoneKeyNum = -1;
		m_pBoneKey = nullptr;
	}

	char		m_Name[NAME_ARRAY_SIZE];//�A�j���[�V������
	int			m_BoneKeyNum;			//�Ή��{�[���̐�
	BONE_KEY* m_pBoneKey;				//�{�[���̃L�[���X�g
};

//���b�V��
struct MESH
{
	MESH()
	{
		m_VerNum = -1;
		m_FaceNum = -1;
		m_MaterialNum = -1;
		m_pFace = nullptr;
		m_pMaterial = nullptr;
		m_pVertexBuffer = nullptr;
		m_pVertex = nullptr;
	}
	int				m_VerNum;		//���_��
	int				m_FaceNum;		//��(�|���S��)��
	FACE* m_pFace;		//�ʃ��X�g
	int				m_MaterialNum;	//�}�e���A���̐�
	MATERIAL* m_pMaterial;	//�}�e���A�����X�g
	ID3D10Buffer* m_pVertexBuffer;//���_�o�b�t�@
	VERTEX* m_pVertex;		//���_���̃��X�g
};

//�X�L�����b�V���w�b�_�[
struct SKIN_MESH_HEADER
{
	SKIN_MESH_HEADER()
	{
		m_MaxVertex = -1;
		m_MaxFace = -1;
		m_BoneNum = -1;
	}
	int	m_MaxVertex;//���_�̍ő�E�F�C�g��
	int m_MaxFace;	//�ʂ̍ő�E�F�C�g��
	int m_BoneNum;	//�{�[����
};

//�X�L�����b�V��
struct SKIN_MESH
{
	SKIN_MESH()
	{
		m_Mesh = MESH();
		m_BoneNum = -1;
		m_pBone = nullptr;
		m_SkinHeader = SKIN_MESH_HEADER();
		m_WeightNum = -1;
		m_pWeight = nullptr;
		m_AnimeNum = -1;
		m_pAnimation = nullptr;
		m_pRoot = nullptr;
	}

	MESH				m_Mesh;			//���b�V��
	int					m_BoneNum;		//�{�[����
	BONE* m_pBone;		//�{�[�����X�g
	SKIN_MESH_HEADER	m_SkinHeader;	//�X�L�����b�V���w�b�_�[
	int					m_WeightNum;	//�E�F�C�g��
	SKIN_WEIGHT* m_pWeight;		//�X�L���E�F�C�g
	int					m_AnimeNum;		//�A�j���[�V������
	ANIMATION* m_pAnimation;	//�A�j���[�V����
	BONE* m_pRoot;		//���[�g�{�[��
};


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
	KEY FrameComplement(int NowFrame, BONE_KEY BoneKey);
	SKIN_MESH m_SkinMesh;	//�X�L�����b�V��
};