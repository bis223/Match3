#pragma once

#include "Texture.h"
#include <string>
#include <list>
#include <map>

struct Asset
{
	std::string assetName;
	void *data;
	//size_t size;
};

struct PendingAsset
{
	std::string assetPath;
	std::string assetName;
};


class AssetLoader
{
private:

	typedef std::list<PendingAsset> PendingAssetList;
	typedef std::map<std::string, Asset> AssetMap;

public:
	AssetLoader();
	~AssetLoader();
	void Update();
	void UnloadAll();
	inline const bool IsLoading() const { return m_IsLoading; }

	template <typename T>
	void Load(const std::string& assetPath, const std::string& assetName)
	{
		static_assert(false);  // un-implemented type
	}

	template<>
	void Load<Texture>(const std::string& assetPath, const std::string& assetName)
	{
		PendingAsset pendingAsset;
		pendingAsset.assetPath = assetPath;
		pendingAsset.assetName = assetName;

		m_PendingTextures.push_back(pendingAsset);
	}


	template <typename T>
	void LoadAsset(const PendingAsset& pendingAsset)
	{
		static_assert(false); // un-implemented type
	}

	template<>
	void LoadAsset<Texture>(const PendingAsset& pendingAsset)
	{
		Texture* tex = new Texture(pendingAsset.assetPath);

		Asset asset;
		asset.assetName = pendingAsset.assetName;
		asset.data = tex;

		m_LoadedAssets.insert(std::make_pair(pendingAsset.assetName, asset));
	}

	template <typename T>
	T* GetAsset(const std::string& assetName) const
	{
		static_assert(false);  // un-implemented type
	}

	template <>
	Texture* GetAsset<Texture>(const std::string& assetName) const
	{
		AssetMap::const_iterator assetIt = m_LoadedAssets.find(assetName);
		if (assetIt == m_LoadedAssets.end())
		{
			//static_assert(false);
			return nullptr;
		}

		void* data = const_cast<void*>(assetIt->second.data);
		Texture* tex = reinterpret_cast<Texture*>(data);

		if(tex)
			return tex;

		return nullptr;
	}


private:

	PendingAssetList m_PendingTextures;
	AssetMap  m_LoadedAssets;
	bool m_IsLoading;
};
