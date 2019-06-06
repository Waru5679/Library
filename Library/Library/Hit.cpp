#include "Hit.h"
#include "Math.h"

 CHit g_Hit;

//ì¬
SphereData CHit::Create(D3DXVECTOR3 vPos, float fRadius,D3DXVECTOR3 vScale)
{
	SphereData data;
	data.m_vPos = vPos;
	data.m_fRadius=fRadius;
	
	return data;
}

//“o˜^
void CHit::Insert(SphereData* pData)
{
	m_data.push_back(pData);
}

//XV
void CHit::UpData(SphereData* pData,D3DXVECTOR3 vPos)
{
	pData->m_vPos = vPos;
}

//‹…Õ“Ë”»’è
bool CHit::SphereHit()
{
	return SphereAndSphre(m_data[0], m_data[1]);
}

//‹…‚Æ‹…‚ÌÕ“Ë”»’è
bool CHit::SphereAndSphre(SphereData* data1, SphereData* data2)
{
	//2“_ŠÔ‚Ì‹——£
	float PosLength;	
	PosLength = D3DXVec3Length(&(data1->m_vPos - data2->m_vPos));
	
	//‚Q‚Â‚Ì”¼Œa‚Ì˜a
	float RadiusLength;
	RadiusLength = data1->m_fRadius + data2->m_fRadius;

	//2“_ŠÔ‚Ì‹——£‚ª”¼Œa‚Ì˜a‚æ‚è¬‚³‚¯‚ê‚Î“–‚½‚Á‚Ä‚¢‚é
	if (PosLength <= RadiusLength)
	{
		return true;
	}

	return false;
}
