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
		D3DXVECTOR3(-1.0f, 1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,0.0f),	//左上
		D3DXVECTOR3(-1.0f, -1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,1.0f),	//左下
		D3DXVECTOR3(1.0f, 1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,0.0f),	//右上
		D3DXVECTOR3(1.0f, -1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,1.0f)	//右下
	};
	m_Vertex = ver;

	//バーテックスバッファ作成
	CreateBartex();
}

//描画 中継
void CDraw::Draw3D(int TexId, D3DXMATRIX matWorld) 
{	
	Draw3D(g_Task.GetTex(TexId)->m_pTex, matWorld);
}

//描画
void CDraw::Draw3D(ID3D10ShaderResourceView* pResView, D3DXMATRIX matWorld)
{
	//描画色
	float Color[4] = { 1.0f,1.0f,1.0f,1.0f };

	//テクスチャ切り取り位置
	float Src[4] = { 0.0f,0.0f,1.0f,1.0f };

	//シェーダーのセット
	g_Shader.SetShader(pResView,Src,Color,matWorld);

	//ポリゴンの描画
	DrawPolygon();
}

//2D描画
void CDraw::Draw2D(ID3D10ShaderResourceView* pTex, RECT_F* pOut,float fRad)
{
	//切り取り（等倍）
	float fSrc[4] = {0.0f,0.0f,1.0f,1.0f};

	//描画色
	float fColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	
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
	D3DXVECTOR2 Size;
	Size.x = pOut->m_right - pOut->m_left;
	Size.y = pOut->m_bottom - pOut->m_top;
	matWorld._11 = Size.x / WINDOW_WIDTH;
	matWorld._22 = Size.y / WINDOW_HEIGHT;

	//Z軸回転行列
	D3DXMATRIX matRot;
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(fRad));

	//ワールドに回転をかける
	matWorld *= matRot;

	//平行移動量を求める
	matWorld._41 = (2.0f / (float)WINDOW_WIDTH) * (pOut->m_left + Size.x / 2.0f) - 1.0f;
	matWorld._42 = (2.0f / (float)WINDOW_HEIGHT) * (pOut->m_top - Size.y / 2.0f) + 1.0f;

	//シェーダーのセット
	g_Shader.SetShader(pTex, fSrc, fColor, matWorld *matInvProj*matInvView);

	//ポリゴンの描画
	DrawPolygon();
}

//2D描画
void CDraw::Draw2D(int TexId, RECT_F* pSrc,RECT_F* pOut,float Color[4],float fRad)
{
	//テクスチャ情報
	MY_TEXTURE* pTex;
	pTex=g_Task.GetTex(TexId);

	//切り取り位置の設定
	float fSrc[4];
	fSrc[0] = pSrc->m_left	/ pTex->m_Width;
	fSrc[1] = pSrc->m_top	/ pTex->m_Height;
	fSrc[2] = pSrc->m_right	/ pTex->m_Width;
	fSrc[3] = pSrc->m_bottom/ pTex->m_Height;

	//逆ビュー行列
	D3DXMATRIX matInvView;	
	D3DXMatrixIdentity(&matInvView);
	D3DXMatrixInverse(&matInvView,NULL,& m_pCamera->GetViewMatrix());

	//逆プロジェクション行列
	D3DXMATRIX matInvProj;
	D3DXMatrixIdentity(&matInvProj);
	D3DXMatrixInverse(&matInvProj, NULL, &m_pCamera->GetProjMatrix());

	//ワールド行列
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	//サイズを求める
	D3DXVECTOR2 Size;
	Size.x = pOut->m_right - pOut->m_left;
	Size.y = pOut->m_bottom - pOut->m_top;
	matWorld._11 =Size.x/WINDOW_WIDTH;
	matWorld._22 = Size.y / WINDOW_HEIGHT;

	//Z軸回転行列
	D3DXMATRIX matRot;
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(fRad));

	//ワールドに回転をかける
	matWorld *= matRot;

	//平行移動量を求める
	matWorld._41 = (2.0f / (float)WINDOW_WIDTH ) * (pOut->m_left + Size.x/2.0f) -1.0f;
	matWorld._42 = (2.0f / (float)WINDOW_HEIGHT) * (pOut->m_top  - Size.y/2.0f) +1.0f;
	
	//シェーダーのセット
	g_Shader.SetShader(pTex->m_pTex,fSrc,Color,matWorld *matInvProj*matInvView);

	//ポリゴンの描画
	DrawPolygon();
}

//バーテックスバッファー作成
HRESULT CDraw::CreateBartex()
{
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MY_VERTEX) * 4;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_Vertex;
	if (FAILED(dx.m_pDevice->CreateBuffer(&bd, &InitData, &m_pBuffer)))
		return FALSE;

	return S_OK;
}

//ポリゴンの描画
void CDraw::DrawPolygon()
{
	//バーテックスバッファーをセット
	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;
	dx.m_pDevice->IASetVertexBuffers(0, 1, &m_pBuffer, &stride, &offset);

	//プリミティブ・トポロジーをセット
	//四角形
	dx.m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//プリミティブをレンダリング
	D3D10_TECHNIQUE_DESC dc;
	g_Shader.m_pTechnique->GetDesc(&dc);
	for (UINT p = 0; p < dc.Passes; ++p)
	{
		g_Shader.m_pTechnique->GetPassByIndex(p)->Apply(0);
		dx.m_pDevice->Draw(4, 0);
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
	m_pBuffer->Release();
}

//RECT_Fのセット
void RectSet(float x, float y, float Size_x, float Size_y, RECT_F* pOut)
{
	pOut->m_top = y;
	pOut->m_bottom = pOut->m_top + Size_y;
	pOut->m_left = x;
	pOut->m_right = pOut->m_left + Size_x;
}
