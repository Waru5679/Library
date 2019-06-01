#include "Polygon.h"
#include "DirectX.h"
#include "Task.h"
#include "Shader.h"
#include "Camera.h"
#include "Math.h"
#include "Main.h"

CDraw g_Draw;

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

//2D描画(Fontなど切り取り位置のないもの用の中継)
void CDraw::DrawTexture(ID3D10ShaderResourceView* pTex, RECT_F* pOut, float fRad)
{
	//切り取り（等倍）
	float fSrc[4] = {0.0f,0.0f,1.0f,1.0f};

	//描画色
	float fColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	
	DrawTexture(pTex, fSrc, pOut, fColor, fRad);
}

//2D描画(テクスチャのサイズから切り取り位置の設定がいるもの用中継)
void CDraw::DrawTexture(int TexId, RECT_F* pSrc,RECT_F* pOut,float fColor[4],float fRad)
{
	//テクスチャ情報
	MY_TEXTURE* pTex=nullptr;
	pTex=g_Task.GetTex(TexId);

	//切り取り位置の設定
	float fSrc[4];
	fSrc[0] = pSrc->m_left	/ pTex->m_Width;
	fSrc[1] = pSrc->m_top	/ pTex->m_Height;
	fSrc[2] = pSrc->m_right	/ pTex->m_Width;
	fSrc[3] = pSrc->m_bottom/ pTex->m_Height;

	DrawTexture(pTex->m_pTex, fSrc, pOut, fColor, fRad);
}

//2D描画
void CDraw::DrawTexture(ID3D10ShaderResourceView* pTex, float fSrc[4], RECT_F* pOut, float Color[4], float fRad)
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
	OutSize.x = pOut->m_right - pOut->m_left;
	OutSize.y = pOut->m_bottom - pOut->m_top;
	matWorld._11 = OutSize.x / WINDOW_WIDTH;
	matWorld._22 = OutSize.y / WINDOW_HEIGHT;

	//Z軸回転行列
	D3DXMATRIX matRot;
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(fRad));

	//ワールドに回転をかける
	matWorld *= matRot;

	//1ピクセルの大きさを求める
	D3DXVECTOR2 PixcelSize;
	PixcelSize.x = (2.0f / (float)WINDOW_WIDTH);
	PixcelSize.y = (2.0f / (float)WINDOW_HEIGHT);

	//マイナスが上に来るように上下反転する
	pOut->m_top = WINDOW_HEIGHT - pOut->m_bottom;
	pOut->m_bottom = WINDOW_HEIGHT - pOut->m_top;

	//平行移動量を求める
	matWorld._41 = PixcelSize.x * pOut->m_left + PixcelSize.x * (OutSize.x / 2.0f) - 1.0f;
	matWorld._42 = PixcelSize.y * pOut->m_top + PixcelSize.y * (OutSize.y / 2.0f) - 1.0f;

	//シェーダーのセット
	g_Shader.SetShader(pTex, fSrc, Color, matWorld *matInvProj*matInvView);

	//ポリゴンの描画
	DrawPolygon(4,m_pVertexBuffer,NULL);
}

//バーテックスバッファー作成
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
	if (FAILED(dx.m_pDevice->CreateBuffer(&bd, &InitData, &pOut)))
		return nullptr;

	return pOut;
}

//ポリゴンの描画
void CDraw::DrawPolygon(int VerNum, ID3D10Buffer* VertexBuffer, ID3D10Buffer* IndexBuffer)
{
	//バーテックスバッファーをセット
	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;
	dx.m_pDevice->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

	//インデックスバッファーをセット
	if (IndexBuffer != NULL)
	{
		stride = sizeof(int);
		offset = 0;
		dx.m_pDevice->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	//プリミティブ・トポロジーをセット
	if (VerNum == 3)
	{
		//三角形
		dx.m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	if (VerNum == 4)
	{
		//四角形
		dx.m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

	//プリミティブをレンダリング
	D3D10_TECHNIQUE_DESC dc;
	g_Shader.m_pTechnique->GetDesc(&dc);
	for (UINT p = 0; p < dc.Passes; ++p)
	{
		g_Shader.m_pTechnique->GetPassByIndex(p)->Apply(0);
		dx.m_pDevice->Draw(VerNum, 0);
	}
}

//テクスチャ読み込み
void CDraw::LoadTexture(int Id,const char* Name,int Width,int Height)
{
	ID3D10ShaderResourceView* pTex;
	//テクスチャーを作成
	D3DX10CreateShaderResourceViewFromFileA(dx.m_pDevice, Name, NULL, NULL, &pTex, NULL);

	//登録
	g_Task.Insert(pTex, Id,Width,Height);
}

//解放
void CDraw::Release()
{
	m_pVertexBuffer->Release();
}
