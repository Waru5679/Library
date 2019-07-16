#include "ModelManager.h"

//ƒ‚ƒfƒ‹Žæ“¾
CModelData* CModelManager::GetModelData(int id)
{
	for (auto itr = m_List.begin(); itr != m_List.end(); itr++)
	{
		if ((*itr).m_Id == id)
			return &(*itr);
	}

	return nullptr;
}