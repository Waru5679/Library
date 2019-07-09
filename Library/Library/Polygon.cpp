#include "LibraryHeader.h"

//インスタンス
CDraw* CDraw::m_pInstance = nullptr;

//初期化
void CDraw::Init()
{
	//頂点情報
	MY_VERTEX ver[]=
	{
		D3DXVECTOR3(-1.0f,  1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,0.0f),	//左上
		D3DXVECTOR3( 1.0f,  1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,0.0f),	//右上
		D3DXVECTOR3(-1.0f, -1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,1.0f),	//左下
		D3DXVECTOR3( 1.0f, -1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,1.0f)	//右下
	};
	
	//バーテックスバッファ作成
	m_pVertexBuffer= BufferCreate(ver,sizeof(MY_VERTEX)*4, D3D10_BIND_VERTEX_BUFFER);
}

//2D描画(テクスチャのサイズから切り取り位置の設定がいるもの用中継)
void CDraw::DrawTexture(int TexId, RECT_F* pSrc,RECT_F* pOut,ColorData* pColor,float fRad)
{
	//テクスチャ情報
	MY_TEXTURE* pTex=nullptr;
	pTex= g_Task.GetTex(TexId);

	//切り取り位置の設定
	RECT_F Src;
	Src.m_Top	= pSrc->m_Left	/ pTex->m_Width;
	Src.m_Left	= pSrc->m_Top	/ pTex->m_Height;
	Src.m_Right = pSrc->m_Right	/ pTex->m_Width;
	Src.m_Bottom= pSrc->m_Bottom/ pTex->m_Height;

	DrawTexture(pTex->m_pTex, &Src, pOut, pColor, fRad);
}

//2D描画
void CDraw::DrawTexture(ID3D10ShaderResourceView* pTex, RECT_F* pSrc, RECT_F* pOut, ColorData* pColor, float fRad)
{
	//逆ビュー行列
	D3DXMATRIX matInvView;
	D3DXMatrixIdentity(&matInvView);
	D3DXMatrixInverse(&matInvView, NULL, &m_pCamera->GetViewMatrix());

	//逆プロジェクション行列
	D3DXMATRIX matInvProj;
	D3DXMatrixIdentity(&matInvProj);
	D3DXMatrixInverse(&matInvProj, NULL, &m_pCamera->GetProjMatrix());

	//ワールド行列
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	//サイズを求める
	D3DXVECTOR2 OutSize;
	OutSize.x = pOut->m_Right - pOut->m_Left;
	OutSize.y = pOut->m_Bottom - pOut->m_Top;
	matWorld._11 = OutSize.x / WINDOW_WIDTH;
	matWorld._22 = OutSize.y / WINDOW_HEIGHT;

	//Z軸回転行列
	D3DXMATRIX matRot;
	D3DXMatrixRotationZ(&matRot, (float)D3DXToRadian(fRad));
	
	//ワールドに回転をかける
	matWorld *= matRot;

	//1ピクセルの大きさを求める
	D3DXVECTOR2 PixcelSize;
	PixcelSize.x = (2.0f / (float)WINDOW_WIDTH);
	PixcelSize.y = (2.0f / (float)WINDOW_HEIGHT);

	//マイナスが上に来るように上下反転する
	pOut->m_Top = WINDOW_HEIGHT - pOut->m_Bottom;
	pOut->m_Bottom = WINDOW_HEIGHT - pOut->m_Top;

	//平行移動量を求める
	matWorld._41 = PixcelSize.x * pOut->m_Left + PixcelSize.x * (OutSize.x / 2.0f) - 1.0f;
	matWorld._42 = PixcelSize.y * pOut->m_Top + PixcelSize.y * (OutSize.y / 2.0f) - 1.0f;

	//シェーダーのセット
	SHADER->SetShader(pTex, pSrc, pColor, matWorld *matInvProj*matInvView);

	//ポリゴンの描画
	DrawPolygon(4,m_pVertexBuffer,NULL);
}

//バッファー作成
ID3D10Buffer* CDraw::BufferCreate(void* pHead, unsigned int Size, int BufferType)
{
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = Size;
	bd.BindFlags = BufferType;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pHead;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	//バッファ作成
	ID3D10Buffer* pOut;
	if (FAILED(DX->GetDevice()->CreateBuffer(&bd, &InitData, &pOut)))
		return nullptr;

	return pOut;
}

//ポリゴンの描画
void CDraw::DrawPolygon(int VerNum, ID3D10Buffer* VertexBuffer, ID3D10Buffer* IndexBuffer)
{
	//バーテックスバッファーをセット
	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;
	DX->GetDevice()->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

	//インデックスバッファーをセット
	if (IndexBuffer != NULL)
	{
		stride = sizeof(int);
		offset = 0;
		DX->GetDevice()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	//プリミティブ・トポロジーをセット
	if (VerNum == 3)
	{
		//三角形
		DX->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	if (VerNum == 4)
	{
		//四角形
		DX->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

	//プリミティブをレンダリング
	D3D10_TECHNIQUE_DESC dc;
	SHADER->GetTechnique()->GetDesc(&dc);
	for (UINT p = 0; p < dc.Passes; ++p)
	{
		SHADER->GetTechnique()->GetPassByIndex(p)->Apply(0);
		DX->GetDevice()->Draw(VerNum, 0);
	}
}

//テクスチャ読み込み
void CDraw::LoadTexture(int Id,const char* Name,int Width,int Height)
{
	ID3D10ShaderResourceView* pTex;
	//テクスチャーを作成
	D3DX10CreateShaderResourceViewFromFileA(DX->GetDevice(), Name, NULL, NULL, &pTex, NULL);

	//登録
	g_Task.Insert(pTex, Id,Width,Height);
}

//解放
void CDraw::Release()
{
	m_pVertexBuffer->Release();
	
	//インスタンス破棄
	PointerRelease(m_pInstance);
}