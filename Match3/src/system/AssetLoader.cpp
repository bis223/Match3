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
	m_IsLoading = !m_PendingAssets.empty();
	if (!m_IsLoading)
		return;

	PendingAsset assetToLoad = m_PendingAssets.front();
	m_PendingAssets.pop_front();
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
