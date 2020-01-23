#pragma once

struct	BDEF1
{
	BDEF1()
	{
		m_BoneID = -1;
	}
	int m_BoneID;
};

struct	BDEF2
{
	BDEF2()
	{
		m_BoneID[0] = -1;
		m_BoneID[1] = -1;
	}
	int m_BoneID[2];
	float m_Weight;
};

struct	BDEF4
{
	BDEF4()
	{
		m_BoneID[0] = -1;
		m_BoneID[1] = -1;
		m_BoneID[2] = -1;
		m_BoneID[3] = -1;
	}

	int m_BoneID[4];
	float m_Weight[4];
};

struct SDEF
{
	SDEF()
	{
		m_BoneID[0] = -1;
		m_BoneID[1] = -1;
	}
	int m_BoneID[2];
	float m_Weight;
	float m_Matrix[3][3];
};

//ウェイトデータ
struct PMX_WEIGHT_DATA
{
	PMX_WEIGHT_DATA()
	{
		m_pBdef1 = nullptr;
		m_pBdef2 = nullptr;
		m_pBdef4 = nullptr;
		m_pSdef = nullptr;
	}
	~PMX_WEIGHT_DATA()
	{
		//BDEF1
		if (m_pBdef1 != nullptr)
		{
			delete m_pBdef1;
			m_pBdef1 = nullptr;
		}
		//BDEF2
		if (m_pBdef2 != nullptr)
		{
			delete m_pBdef2;
			m_pBdef2 = nullptr;
		}
		//BDEF4
		if (m_pBdef4 != nullptr)
		{
			delete m_pBdef4;
			m_pBdef4 = nullptr;
		}
		//SDEF
		if (m_pBdef1 != nullptr)
		{
			delete m_pSdef;
			m_pSdef = nullptr;
		}
	}
	unsigned char m_WeightType;//ウェイト変形方式

	BDEF1* m_pBdef1;	//BDEF1形式のデータ
	BDEF2* m_pBdef2;	//BDEF2形式のデータ
	BDEF4* m_pBdef4;	//BDEF4形式のデータ
	SDEF* m_pSdef;		//SDEF形式のデータ
};

//pmxヘッダー
struct PMX_HEADER
{
	~PMX_HEADER()
	{
		if (m_pData != nullptr)
		{
			delete[] m_pData;
			m_pData = nullptr;
		}
	}
	char	m_FileType[4];	//ファイル種類"PMX_"固定
	char	m_Size;			//データサイズ
	float	m_Ver;			//バージョン
	unsigned char* m_pData;
};

//pmxモデル情報
struct PMX_MODEL_INFO
{
	PMX_MODEL_INFO()
	{
		m_pNameJap = nullptr;
		m_pNameEng = nullptr;
		m_pCommentJap = nullptr;
		m_pCommentEng = nullptr;
	}
	~PMX_MODEL_INFO()
	{
		//モデル名(日)
		if (m_pNameJap != nullptr)
		{
			delete[] m_pNameJap;
			m_pNameJap = nullptr;
		}
		//モデル名(英)
		if (m_pNameEng != nullptr)
		{
			delete[] m_pNameEng;
			m_pNameEng = nullptr;
		}
		//コメント(日)
		if (m_pCommentJap != nullptr)
		{
			delete[] m_pCommentJap;
			m_pCommentJap = nullptr;
		}
		//コメント(英)
		if (m_pCommentEng != nullptr)
		{
			delete[] m_pCommentEng;
			m_pCommentEng = nullptr;
		}
	}
	wchar_t* m_pNameJap;	//モデル名(日)
	wchar_t* m_pNameEng;	//モデル名(英)
	wchar_t* m_pCommentJap;	//コメント(日)
	wchar_t* m_pCommentEng;	//コメント(英)
};

//pmx頂点データ
struct PMX_VERTEX
{
	PMX_VERTEX()
	{
		m_pfAddUv = nullptr;
	}
	~PMX_VERTEX()
	{
		//追加UV
		if (m_pfAddUv != nullptr)
		{
			delete[] m_pfAddUv;
			m_pfAddUv = nullptr;
		}
	}

	float			m_fPos[3];		//位置
	float			m_fNorm[3];		//法線
	float			m_fUv[2];		//UV
	float*			m_pfAddUv;		//追加UV
	float			m_EdgeMagn;		//エッジ倍率
	PMX_WEIGHT_DATA m_WeightData;	//ウェイトデータ
};

//pmx面データ
struct PMX_FACE
{
	int m_VerIndex[3];//頂点
};

//pmxテクスチャ
struct PMX_TEXTURE
{
	PMX_TEXTURE()
	{
		m_pPass = nullptr;
	}
	~PMX_TEXTURE()
	{
		//テクスチャパス
		if (m_pPass != nullptr)
		{
			delete[] m_pPass;
			m_pPass = nullptr;
		}
	}
	wchar_t* m_pPass;	//テクスチャパス
};

//pmxマテリアル
struct PMX_MATERIAL
{
	PMX_MATERIAL()
	{
		m_pNameEng = nullptr;
		m_pNameJap = nullptr;
		m_pMemo = nullptr;
	}
	~PMX_MATERIAL()
	{
		//材質名(日)
		if (m_pNameJap != nullptr)
		{
			delete[] m_pNameJap;
			m_pNameJap = nullptr;
		}
		//材質名(英)
		if (m_pNameEng != nullptr)
		{
			delete[] m_pNameEng;
			m_pNameEng = nullptr;
		}
		//メモ
		if (m_pMemo != nullptr)
		{
			delete[] m_pMemo;
			m_pMemo = nullptr;
		}
	}

	wchar_t*		m_pNameJap;		//材質名(日)
	wchar_t*		m_pNameEng;		//材質名(英)
	wchar_t*		m_pMemo;		//メモ	
	unsigned char	m_BitFlag;		//描画フラグ( 0x01:両面描画, 0x02:地面影, 0x04:セルフシャドウマップへの描画, 0x08:セルフシャドウの描画, 0x10:エッジ描画)
	unsigned char	m_SphereMode;	//スフィアモード(0:無効 1:乗算(sph) 2:加算(spa) 3:サブテクスチャ)
	unsigned char	m_ToonFlag;		//共有Toonフラグ
	float			m_Diffuse[4];	//拡散光
	float			m_Specular[3];	//スペキュラー（鏡面反射
	float			m_SpePower;		//スペキュラーパワー
	float			m_Ambient[3];	//アンビエント
	float			m_Edge[4];		//エッジ色
	float			m_EdgeSize;		//エッジサイズ
	int				m_NormTex;		//通常テクスチャインデックス
	int				m_SphereTex;	//スフィアテクスチャインデックス
	int				m_ToonTex;		//トゥーンテクスチャインデックス
	int				m_UseVerNum;	//使用する頂点数
};

//pmxIKリンク
struct PMX_IK_LINK
{
	unsigned char m_RadRest;//角度制限の有無

	int		m_LinkBoneId;	//リンクボーンID
	float	m_fLowerRad[3];	//下限角度
	float	m_fUpperRad[3];	//上限角度
};

//pmxIK
struct PMX_IK
{
	PMX_IK()
	{
		m_pLink = nullptr;
	}
	~PMX_IK()
	{
		delete[] m_pLink;
		m_pLink = nullptr;
	}
	int		m_TargetId;	//ターゲットボーンId
	int		m_RoopTime;	//回転数
	int		m_LinkNum;	//リンク数

	float	m_fRad;		//回転角度

	PMX_IK_LINK* m_pLink;//リンクデータ
};

//pmxボーン
struct PMX_BONE
{
	PMX_BONE()
	{
		m_pNameJap = nullptr;
		m_pNameEng = nullptr;
	}
	~PMX_BONE()
	{
		//ボーン名(日)
		if (m_pNameJap != nullptr)
		{
			delete[] m_pNameJap;
			m_pNameJap = nullptr;
		}
		//ボーン名(英)
		if (m_pNameEng != nullptr)
		{
			delete[] m_pNameEng;
			m_pNameEng = nullptr;
		}
	}
	wchar_t*		m_pNameJap;		//ボーン名(日)
	wchar_t*		m_pNameEng;		//ボーン名(英)
	unsigned char	m_BitFlag[2];	//ビットフラグ
	int				m_ParentId;		//親インデックス
	int				m_Hierarchy;	//変形階層
	int				m_ConnectId;	//接続先ボーンID
	int				m_GrantId;		//付与親ボーン
	int				m_Key;			//Key
	float			m_fOffset[3];	//オフセット
	float			m_fGrantRate;	//付与率
	float			m_fPos[3];		//位置
	float			m_fFixedAxis[3];//固定軸
	float			m_fAxisX[3];	//X軸
	float			m_fAxisZ[3];	//Z軸
	PMX_IK			m_Ik;			//IKデータ
};


//pmx頂点モーフ
struct PMX_VER_MORPH
{
	int		m_VerId;		//頂点ID
	float	m_fOffset[3];	//座標オフセット値
};

//pmxUVモーフ
struct PMX_UV_MORPH
{
	int		m_VerId;		//頂点ID
	float	m_fOffset[4];	//UVオフセット
};

//pmxボーンモーフ
struct PMX_BONE_MORPH
{
	int		m_BoneId;	//ボーンID
	float	m_fMove[3];	//移動量
	float	m_fRot[4];	//回転量
};

//pmx材質モーフ
struct PMX_MATE_MORPH
{
	unsigned char m_Format;//演算形式

	int		m_MateId;		//対応マテリアルID(-1は全マテリアル対応)
	float	m_fDiffuse[4];	//拡散光
	float	m_fSpecular[3];	//スペキュラー（鏡面反射
	float	m_fSpePower;	//スペキュラーパワー
	float	m_fAmbient[3];	//アンビエント
	float	m_fEdge[4];		//エッジ色
	float	m_fEdgeSize;	//エッジサイズ
	float	m_fTex[4];		//テクスチャ係数
	float	m_fSphereTex[4];//スフィアテクスチャ係数
	float	m_fToonTex[4];	//Toonテクスチャ係数
};

//グループモーフ
struct PMX_GROUP_MORPH
{
	int		m_MorphId;	//モーフID
	float	m_fRate;	//モーフ率
};

//pmxモーフ
struct PMX_MORPH
{
	PMX_MORPH()
	{
		m_pNameJap = nullptr;
		m_pNameEng = nullptr;
		m_pVerMorph = nullptr;
		m_pUvMorph = nullptr;
		m_pBoneMorph = nullptr;
		m_pMateMorph = nullptr;
		m_pGroupMorph = nullptr;
	}
	~PMX_MORPH()
	{
		//モーフ名(日)
		if (m_pNameJap != nullptr)
		{
			delete[] m_pNameJap;
			m_pNameJap = nullptr;
		}
		//モーフ名(英)
		if (m_pNameEng != nullptr)
		{
			delete[] m_pNameEng;
			m_pNameEng = nullptr;
		}
		//頂点モーフ
		if (m_pVerMorph != nullptr)
		{
			delete[] m_pVerMorph;
			m_pVerMorph = nullptr;
		}
		//UVモーフ
		if (m_pUvMorph != nullptr)
		{
			delete[] m_pUvMorph;
			m_pUvMorph = nullptr;
		}
		//ボーンモーフ
		if (m_pBoneMorph != nullptr)
		{
			delete[] m_pBoneMorph;
			m_pBoneMorph = nullptr;
		}
		//材質モーフ
		if (m_pMateMorph != nullptr)
		{
			delete[] m_pMateMorph;
			m_pMateMorph = nullptr;
		}
		//グループモーフ
		if (m_pGroupMorph != nullptr)
		{
			delete[] m_pGroupMorph;
			m_pGroupMorph = nullptr;
		}
	}

	wchar_t*			m_pNameJap;		//モーフ名(日)
	wchar_t*			m_pNameEng;		//モーフ名(英)
	unsigned char		m_PmdType;		//PMDカテゴリー(0:システム予約,1:眉(左下),2:目(左上),3:口(右上),4:その他(右下)) 
	unsigned char		m_MorphType;	//モーフタイプ(0:グループ,1:頂点,2:ボーン,3:UV,4-7:追加UV,8:材質)
	int					m_DataNum;		//データ数
	PMX_VER_MORPH*		m_pVerMorph;	//頂点モーフ
	PMX_UV_MORPH*		m_pUvMorph;		//UVモーフ
	PMX_BONE_MORPH*		m_pBoneMorph;	//ボーンモーフ
	PMX_MATE_MORPH*		m_pMateMorph;	//材質モーフ
	PMX_GROUP_MORPH*	m_pGroupMorph;	//グループモーフ
};

//枠内要素
struct FRAME_ELEMENT
{
	unsigned char	m_Flag;		//0:ボーン 1:モーフ
	int				m_Index;	//ボーンまたはモーフのインデックス
};

//表示枠
struct PMX_DISPLAY
{
	PMX_DISPLAY()
	{
		m_pNameJap = nullptr;
		m_pNameEng = nullptr;
	}
	~PMX_DISPLAY()
	{
		//表示枠名(日)
		if (m_pNameJap != nullptr)
		{
			delete[] m_pNameJap;
			m_pNameJap = nullptr;
		}
		//表示枠名(英)
		if (m_pNameEng != nullptr)
		{
			delete[] m_pNameEng;
			m_pNameEng = nullptr;
		}
		//枠内要素数
		if (m_pElement != nullptr)
		{
			delete[] m_pElement;
			m_pElement = nullptr;
		}
	}
	wchar_t*		m_pNameJap;		//表示枠名(日)
	wchar_t*		m_pNameEng;		//表示枠名(英)
	unsigned char	m_SpecialFlag;	//特殊フラグ
	int				m_ElementNum;	//要素数
	FRAME_ELEMENT*	m_pElement;		//枠内要素数
};

//剛体
struct PMX_RIGIT_BODY
{
	PMX_RIGIT_BODY()
	{
		m_pNameJap = nullptr;
		m_pNameEng = nullptr;
	}
	~PMX_RIGIT_BODY()
	{
		//剛体名(日)
		if (m_pNameJap != nullptr)
		{
			delete[] m_pNameJap;
			m_pNameJap = nullptr;
		}
		//剛体名(英)
		if (m_pNameJap != nullptr)
		{
			delete[] m_pNameEng;
			m_pNameEng = nullptr;
		}
	}

	wchar_t*		m_pNameJap;		//剛体名(日)
	wchar_t*		m_pNameEng;		//剛体名(英)
	unsigned char	m_Group;		//グループ	
	unsigned char	m_Shape;		//0:球,1:箱,2:カプセル
	unsigned char   m_Operation;	//0:ボーン追従(static) 1:物理演算(dynamic) 2:物理演算 + Bone位置合わせ
	unsigned short	m_NoCollision;	//非衝突グループ
	int				m_BoneId;		//ボーンID
	float			m_fSize[3];		//サイズ
	float			m_fPos[3];		//位置
	float			m_fRad[3];		//回転
	float			m_fMass;		//質量
	float			m_fMoveDecay;	//移動減衰
	float			m_fRotDecay;	//回転減衰
	float			m_fRepulsive;	//反発力
	float			m_fFriction;	//摩擦力
};

//Joint
struct PMX_JOINT
{
	PMX_JOINT()
	{
		m_pNameJap = nullptr;
		m_pNameEng = nullptr;
	}
	~PMX_JOINT()
	{
		//joint名(日
		if (m_pNameJap != nullptr)
		{
			delete[] m_pNameJap;
			m_pNameJap = nullptr;
		}
		//joint名(英)
		if (m_pNameEng != nullptr)
		{
			delete[] m_pNameEng;
			m_pNameEng = nullptr;
		}
	}
	wchar_t*		m_pNameJap;			//joint名(日)
	wchar_t*		m_pNameEng;			//joint名(英)
	unsigned char	m_Type;				//Jointの種類(ver2.0は0のみ)
	int				m_RigitId[2];		//関連剛体のインデックス
	float			m_fPos[3];			//位置
	float			m_fRad[3];			//回転
	float			m_fUpperMove[3];	//移動制限(上限)
	float			m_fLowerMove[3];	//移動制限(下限)
	float			m_fUpperRad[3];		//回転制限(上限)
	float			m_fLowerRad[3];		//回転制限(下限)
	float			m_fSpringMove[3];	//ばね定数_移動
	float			m_fSpringRad[3];	//ばね定数_回転
};
