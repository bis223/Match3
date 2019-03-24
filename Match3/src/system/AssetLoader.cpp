#include "AssetLoader.h"



AssetLoader::AssetLoader()
{
}

AssetLoader::~AssetLoader()
{
	UnloadAll();
}

void AssetLoader::Update()
{
	m_IsLoading = !m_PendingTextures.empty();
	if (!m_IsLoading)
		return;

	PendingAsset assetToLoad = m_PendingTextures.front();
	m_PendingTextures.pop_front();
	LoadAsset<Texture>(assetToLoad);
}
void AssetLoader::UnloadAll()
{
	for (AssetMap::iterator assetIt = m_LoadedAssets.begin(), end = m_LoadedAssets.end();
		assetIt != end;
		++assetIt)
	{
		delete assetIt->second.data;
	}

	m_LoadedAssets.clear();
}
//
//void* AssetLoader::GetAsset(const std::string& assetName) const
//{
//	AssetMap::const_iterator assetIt = m_LoadedAssets.find(assetName);
//	if (assetIt == m_LoadedAssets.end())
//	{
//		//static_assert(false);
//		return nullptr;
//	}
//
//	return assetIt->second;
//}
