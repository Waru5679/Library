#include "ModelManager.h"
#include "LibraryHeader.h"

//インスタンス
CModelManager* CModelManager::m_pInstance = nullptr;

//モデル読み込み
void CModelManager::LoadModel(int Id, const char* Name)
{
	CModelData Mesh;
	Mesh.m_Id = Id;

	//読み込み
	LOADER->LoadObj(Name, &Mesh);

	//登録
	m_List.push_back(Mesh);
}

//モデル取得
CModelData* CModelManager::GetModelData(int id)
{
	for (auto itr = m_List.begin(); itr != m_List.end(); itr++)
	{
		if ((*itr).m_Id == id)
			return &(*itr);
	}
	return nullptr;
}

//モデル描画
void CModelManager::Draw(D3DMATRIX matWorld, CModelData* pMesh, CColorData* pColor)
{
	//マテリアルの数毎に描画
	for (unsigned int i = 0; i < pMesh->m_Material.size(); i++)
	{
		int size = pMesh->m_Material[i].m_pVertex.size();

		//シェーダーのセット
		SHADER->SetShader(pMesh->m_Material[i].m_pTexture, NULL, pColor, matWorld);

		for (int j = 0; j < size; j++)
		{
			//ポリゴン描画
			DRAW->DrawPolygon(pMesh->m_Material[i].m_Face[j].m_Vertex.size(), pMesh->m_Material[i].m_pVertex[j], pMesh->m_Material[i].m_pIndex[j]);
		}
	}
}

//開放
void CModelManager::Release()
{
	//モデルデータ破棄
	for (auto itr = m_List.begin(); itr != m_List.end(); itr++)
	{
		(*itr).Release();
	}
	m_List.clear();

	//インスタンス破棄
	PointerRelease(m_pInstance);
}

//面データ解放
void CFaceData::Release()
{
	//頂点情報
	for (unsigned int i = 0; i < m_Vertex.size(); i++)
	{
		m_Vertex[i].Release();
	}
	VectorRelease(m_Vertex);
}

//マテリアル解放
void CMaterialData::Release()
{
	//面情報
	for (unsigned int i = 0; i < m_Face.size(); i++)
	{
		m_Face[i].Release();
	}
	VectorRelease(m_Face);

	//インデックスバッファー
	for (unsigned int i = 0; i < m_pIndex.size(); i++)
	{
		m_pIndex[i]->Release();
	}
	VectorRelease(m_pIndex);

	//バーテックスバッファー
	for (unsigned int i = 0; i < m_pVertex.size(); i++)
	{
		m_pVertex[i]->Release();
	}
	VectorRelease(m_pVertex);

	//テクスチャ
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

//モデルデータ解放
void CModelData::Release()
{
	//マテリアル
	for (unsigned int i = 0; i < m_Material.size(); i++)
	{
		m_Material[i].Release();
	}
	VectorRelease(m_Material);
}
