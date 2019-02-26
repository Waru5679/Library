#include "Polygon.h"
#include "DirectX.h"
#include "Task.h"
#include "Shader.h"
#include "Camera.h"
#include "Math.h"

CDraw g_Draw;

//初期化
void CDrawPolygon::Init()
{
	//頂点情報
	MY_VERTEX ver[]=
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,0.0f),
		D3DXVECTOR3(0.0f, -0.1f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,1.0f),
		D3DXVECTOR3(0.1f, 0.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,0.0f),
		D3DXVECTOR3(0.1f, -0.1f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,1.0f)
	};
	m_vertex = ver;

	//バーテックスバッファ作成
	CreateBartex();

	//マテリアル
	m_material.Ka = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	m_material.Kd = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	m_material.Ks = D3DXVECTOR3(0.1f, 0.1f, 0.1f);

}

//描画 中継
void CDrawPolygon::Draw3D(int TexId, D3DXMATRIX matWorld) 
{	
	Draw3D(g_Task.GetTex(TexId), matWorld);
}

//描画
void CDrawPolygon::Draw3D(ID3D10ShaderResourceView* pResView, D3DXMATRIX matWorld)
{
	//テクスチャ
	m_material.pTexture = pResView;

	//ワールド行列
	m_matWorld = matWorld;

	//シェーダーのセット
	g_Shader.SetShader(matWorld, m_material);

	//ポリゴンの描画
	DrawPolygon();
}

//2D描画 中継
void CDrawPolygon::Draw2D(int TexId, RECT_F* Out)
{

	Draw2D(g_Task.GetTex(TexId),Out);
}

//2D描画
void CDrawPolygon::Draw2D(ID3D10ShaderResourceView* pResView, RECT_F*Out)
{
	//テクスチャ
	m_material.pTexture = pResView;

	//スクリーン座標から位置を取得
	Camera* pCamera = g_Task.GetCamera(0);
	CalcScreenToWorld(&m_vPos, Out->m_left, Out->m_top, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, &pCamera->GetViewMatrix(), &pCamera->GetProjMatrix());
	
	//スケール
	m_vScale.x = (Out->m_right  - Out->m_left) / 240.0f;
	m_vScale.y = (Out->m_bottom - Out->m_top)/ 240.0f;
	m_vScale.z = 0.0f;

	//カメラの回転を取得
	m_vAngle= pCamera->GetVertical();
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);
	   
	//シェーダーのセット
	g_Shader.SetShader(m_matWorld, m_material);

	//ポリゴンの描画
	DrawPolygon();
}

//バーテックスバッファー作成
HRESULT CDrawPolygon::CreateBartex()
{
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MY_VERTEX) * 4;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_vertex;
	if (FAILED(dx.m_pDevice->CreateBuffer(&bd, &InitData, &m_pBuffer)))
		return FALSE;

	return S_OK;
}

//ポリゴンの描画
void CDrawPolygon::DrawPolygon()
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
	dx.m_pTechnique->GetDesc(&dc);
	for (UINT p = 0; p < dc.Passes; ++p)
	{
		dx.m_pTechnique->GetPassByIndex(p)->Apply(0);
		dx.m_pDevice->Draw(4, 0);
	}
}

//テクスチャ読み込み
void CDrawPolygon::LoadTexture(int Id,const char* Name)
{
	ID3D10ShaderResourceView* pTex;
	//テクスチャーを作成
	D3DX10CreateShaderResourceViewFromFileA(dx.m_pDevice, Name, NULL, NULL, &pTex, NULL);

	//登録
	g_Task.Insert(pTex, Id);
}