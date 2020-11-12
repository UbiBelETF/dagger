#pragma once

#include "core/core.h"
#include "core/system.h"

#include "core/graphics/texture.h"
#include "core/graphics/animation.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace dagger;

template <class Container>
void split(const std::string& str, Container& cont)
{
	std::istringstream iss(str);
	std::copy(std::istream_iterator<std::string>(iss),
		std::istream_iterator<std::string>(),
		std::back_inserter(cont));
}

enum struct PathNodeType
{
	Folder,
	File
};

struct PathNode
{
	virtual void AddPath(String path_, String fullPath_) = 0;
	virtual PathNodeType GetType() = 0;

	PathNode() = default;
	~PathNode() = default;
};

struct Folder : PathNode
{
	Map<String, PathNode*> m_Nodes;

	void AddPath(String path_, String fullPath_) override;
	inline PathNodeType GetType() { return PathNodeType::Folder; }

	Folder() = default;
	~Folder();
};

struct FolderLeaf : PathNode
{
	String m_Leaf;
	FolderLeaf(String leaf_) : m_Leaf{ leaf_ } {}

	void AddPath(String path_, String fullPath_) override;
	inline PathNodeType GetType() { return PathNodeType::File; }
};


template<typename T>
class CreationQueue
{
	Sequence<AssetCreateRequest<T>> m_Requests{};

public:
	inline void Enqueue(AssetCreateRequest<T> request_)
	{
		m_Requests.push_back(request_);
	}

	inline void Dispatch()
	{
		for (auto request : m_Requests)
		{
			Engine::Dispatcher().trigger<AssetCreateRequest<T>>(request);
		}
		m_Requests.clear();
	}
};


class Finder : public System
{
	inline String SystemName() { return "Finder"; }

	CreationQueue<Entity> m_EntityQueue;
	CreationQueue<Texture> m_TextureQueue;
	CreationQueue<Animation> m_AnimationQueue;

	OwningPtr<Folder> m_Structure;
	int m_PropertiesIndex{ 0 };

	void OnToolMenuRender();
	void OnGUIRender();
	void OnRequestCreateEntity(AssetCreateRequest<Entity> entity_);
	void OnRequestCreateTexture(AssetCreateRequest<Texture> texture_);
	void OnRequestCreateAnimation(AssetCreateRequest<Animation> animation_);

	void OnRequestReloadingStructure();

	void SpinUp() override;
	void Run() override;
	void WindDown() override;
};
