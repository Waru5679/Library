#include "Font.h"
#include "DirectX.h"
#include "Math.h"

HFONT		CFont::m_hFont;		//フォントハンドル(論理)
HFONT		CFont::m_oldFont;	//フォントハンドル(物理)
HDC			CFont::m_hdc;		//ディスプレイデバイスコンテキストのハンドル
TEXTMETRIC	CFont::m_TM;		//フォント情報格納用

list<unique_ptr < CCharClass >>* CFont::list_char_tex;	//文字リスト

//文字テクスチャ作成
void CCharClass::CreateCharTex(wchar_t c, HDC hdc, TEXTMETRIC TM)
{
	//識別文字用
	UINT code = 0;	//作成する文字コード

	//文字フォント描画用
	BYTE* ptr;			//文字のビットマップを入れる
	DWORD size;			//メモリサイズ
	GLYPHMETRICS GM;	//象形文字の情報が格納
	const MAT2 Mat = { {0,1}, {0,0},{0,0},{0,1} };	//Font書き込みの向き

	//テクスチャ書き込み用ポインタ
	D3D10_MAPPED_TEXTURE2D mapped;
	BYTE* pBits;	//テクスチャのピクセル情報を入れるポインタ

	//識別文字コード登録
	m_pc.reset(new wchar_t(c));
	code = (UINT)*m_pc.get();

	//フォント情報から文字のビットマップ取得
	//文字のビットマップの大きさ取得
	size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat);
	ptr = new BYTE[size];
	
	//文字のビットマップ情報をptrに入れる
	GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &Mat);

	//空テクスチャの設定
	D3D10_TEXTURE2D_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;		//テクスチャフォーマットR8G8B8の24Bit
	desc.SampleDesc.Count = 1;						//サンプリングは1ピクセルのみ
	desc.Usage = D3D10_USAGE_DYNAMIC;				//CPU書き込み可能
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;	//シェーダリソース
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;	//CPUから書き込みアクセス可
	desc.Height = 32;			
	desc.Width = 32;

	//設定を元に空テクスチャを作成
	dx.m_pDevice->CreateTexture2D(&desc, 0, &m_pTexture);

	//テクスチャ情報を取得する
	D3D10_TEXTURE2D_DESC texDesc;
	m_pTexture->GetDesc(&texDesc);

	//テクスチャにShaderResourceViewを接続
	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	dx.m_pDevice->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pTexResView);

	//テクスチャロック
	m_pTexture->Map(
		D3D10CalcSubresource(0, 0, 1),
		D3D10_MAP_WRITE_DISCARD,
		0,
		&mapped	);
	pBits = (BYTE*)mapped.pData;

	//書き出し位置（左上)
	unsigned int iOfs_x = GM.gmptGlyphOrigin.x;
	unsigned int iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
	
	//フォントビットマップのサイズ
	unsigned int iBmp_w = GM.gmBlackBoxX + (4 - (GM.gmBlackBoxX % 4)) % 4;
	unsigned int iBmp_h = GM.gmBlackBoxY;
	
	//α値の段階
	int Level = 17; //GGO_GRAY4＿BITMAPなので17

	DWORD dwColor,dwFontColor,dwBackColor;
	
	//背景色
	dwBackColor = 0x00ffffff;//透過(黒	
	//フォント色
	dwFontColor = 0xffffffff;//白

	//1ピクセル単位にフォントの色情報(32bit)をテクスチャに書き込み
	memset(pBits, 0x00, sizeof(pBits)*32*32);

	for(unsigned int y=0;y<32;y++)
	{
		for (unsigned int x = 0; x < 32; x++)
		{
			//文字の範囲
			if (iOfs_y <= y && y < iOfs_y + iBmp_h
				&& iOfs_x<=x && x< iOfs_x + GM.gmBlackBoxX)
			{
				dwColor = (255 * ptr[x - iOfs_x + iBmp_w * (y - iOfs_y)]) / (Level - 1);

				//色指定
				if (dwColor == 0)
				{
					//空白部
					dwColor = dwBackColor;
				}
				else
				{
					//文字部
					dwColor = dwFontColor;
				}
			}
			else
			{
				//淵
				dwColor = dwBackColor;
			}
			memcpy((BYTE*)pBits + (y << 7) + (x << 2), &dwColor, sizeof(DWORD));
		}
	}
		
	//テクスチャアンロック
	m_pTexture->Unmap(D3D10CalcSubresource(0, 0, 1));

	//文字ビットマップデータ削除
	delete[] ptr;
}


//初期化
void CFont::Init()
{

	//リスト確保＆初期化
	list_char_tex = new list<  unique_ptr < CCharClass >>;
	list_char_tex->clear();

	//unicodeの文字を日本コードにする
	setlocale(LC_CTYPE, "jpn");

	//論理フォント設定
	HFONT hFont = CreateFont(
		32,
		0, 0, 0, 0,
		FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		L"ＭＳ ゴシック"
	);

	//指定されたウインドウのクライアント領域、または画面全体を表す
	//ディスプレイデバイスコンテキストのハンドルを取得
	m_hdc = GetDC(NULL);

	//論理フォントに最も近い物理フォントをセットする
	m_oldFont = (HFONT)SelectObject(m_hdc, hFont);

	//フォントの情報をバッファに格納
	GetTextMetrics(m_hdc, &m_TM);

	//論理フォントをメンバに渡す
	m_hFont = hFont;
}

//削除
void CFont::Release()
{
	list_char_tex->clear();
	DeleteObject(m_oldFont);
	DeleteObject(m_hFont);
	ReleaseDC(NULL, m_hdc);
}

//文字列をもとにテクスチャを作成
void CFont::CreateStrTex(const wchar_t* str)
{
	bool entry;

	//リストに登録済か調べる
	for (unsigned int i = 0; i < wcslen(str); i++)
	{
		entry = false;
		
		//リストから検索
		for (auto itr = list_char_tex->begin(); itr != list_char_tex->end(); itr++)
		{
			//登録された文字とstrの文字を比較
			if (*itr->get()->GetChar()==str[i])
			{
				//登録されている
				entry = true;
			}
		}

		//登録されてなければ、作成する
		if (entry == false)
		{
			//文字テクスチャ作成
			unique_ptr<CCharClass> obj(new CCharClass());
			obj->CreateCharTex(str[i], m_hdc, m_TM);

			//リストに登録
			list_char_tex->push_back(move(obj));
		}
	}
}

//フォント描画
void CFont::DrawStr(const wchar_t* Str, float Pos_x, float Pos_y, float FontSize,float Rad)
{
	//文字列登録
	CreateStrTex(Str);

	for (unsigned int i = 0; i < wcslen(Str); i++)
	{
		//リストから検索
		for (auto itr = list_char_tex->begin(); itr != list_char_tex->end(); itr++)
		{
			//登録された文字とstrの文字を比較
			if (*itr->get()->GetChar() == Str[i])
			{			
				//テクスチャポインタ
				ID3D10ShaderResourceView* pTex = (*itr)->GetTex();
				
				//描画位置
				RECT_F Out;
				RectSet(Pos_x + FontSize * i, Pos_y, FontSize, FontSize,&Out);
			
				//描画
				g_Draw.Draw2D(pTex, &Out, Rad);

			}
		}
	}	
}

