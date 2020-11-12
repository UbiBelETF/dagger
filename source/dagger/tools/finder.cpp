#include "finder.h"
#include "tools/toolmenu.h"
#include "core/engine.h"

#include "core/core.h"
#include "core/graphics/texture.h"
#include "core/graphics/animation.h"
#include "core/graphics/animations.h"
#include "core/graphics/sprite.h"

#include "core/game/transforms.h"

template<typename T>
void RenderMenu(Folder* path_, CreationQueue<T>& queue_)
{
    for (auto [k, v] : path_->m_Nodes)
    {
        if (v->GetType() == PathNodeType::Folder)
        {
            if (ImGui::BeginMenu(k.c_str()))
            {
                RenderMenu<T>((Folder*)v, queue_);
                ImGui::EndMenu();
            }
        }
        else
        {
            auto leaf = ((FolderLeaf*)v)->m_Leaf;
            if (ImGui::MenuItem(leaf.c_str()))
            {
                queue_.Enqueue(AssetCreateRequest<T>{leaf});
            }
        }
    }
}

void Finder::OnToolMenuRender()
{
    if (ImGui::BeginMenu("Finder"))
    {
        if (ImGui::BeginMenu("Entities"))
        {
            if(ImGui::MenuItem("Create"))
            {
                m_EntityQueue.Enqueue(AssetCreateRequest<Entity>{""});
            }
            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::BeginMenu("Textures"))
        {
            PathNode* path = m_Structure->m_Nodes["textures"];
            RenderMenu<Texture>((Folder*)path, m_TextureQueue);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Animations"))
        {
            PathNode* path = m_Structure->m_Nodes["animations"];
            RenderMenu<Animation>((Folder*)path, m_AnimationQueue);
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
}


void Finder::OnGUIRender()
{
    ImGui::PushItemWidth(350);
    ImGui::Begin("Finder");

    auto& reg = Engine::Registry();

    Sequence<NameTag> tags;
    Sequence<const char*> entities;

    ImGui::BeginGroup();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    Camera::PropertiesDrawer();
    ImGui::PopStyleVar();
    ImGui::EndGroup();

    auto foundTags = reg.view<NameTag>();
    
    for (auto entity : foundTags)
    {
        auto& tag = foundTags.get<NameTag>(entity);
        tags.push_back(tag);
        entities.push_back(tag.name.c_str());
    }

    ImGui::PushItemWidth(400);
    ImGui::ListBox("", &m_PropertiesIndex, entities.data(), entities.size(), 5);
    
    if (m_PropertiesIndex < tags.size())
    {
        auto entity = tags[m_PropertiesIndex].id;

        if (reg.has<Transform>(entity))
        {
            ImGui::BeginGroup();
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
            Transform::PropertiesDrawer(entity);
            ImGui::PopStyleVar();
            ImGui::EndGroup();
        }

        if (reg.has<Sprite>(entity))
        {
            ImGui::BeginGroup();
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
            Sprite::PropertiesDrawer(entity);
            ImGui::PopStyleVar();
            ImGui::EndGroup();
        }

        if (reg.has<Animator>(entity))
        {
            ImGui::BeginGroup();
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
            Animator::PropertiesDrawer(entity);
            ImGui::PopStyleVar();
            ImGui::EndGroup();
        }
    }
    ImGui::End();
}

void Finder::OnRequestCreateEntity(AssetCreateRequest<Entity> entity_)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto& tag = reg.emplace<NameTag>(entity);
    tag.id = entity;
    tag.name = fmt::format("Entity (#{})", entity);

    auto& transform = reg.emplace<Transform>(entity);
}

void Finder::OnRequestCreateTexture(AssetCreateRequest<Texture> texture_)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto& tag = reg.emplace<NameTag>(entity);
    tag.id = entity;
    tag.name = fmt::format("{} (#{})", texture_.path, entity);
    auto& sprite = reg.emplace<Sprite>(entity);
    auto& transform = reg.emplace<Transform>(entity);

    AssignSpriteTexture(sprite, texture_.path);
}

void Finder::OnRequestCreateAnimation(AssetCreateRequest<Animation> animation_)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto& tag = reg.emplace<NameTag>(entity);
    tag.id = entity;
    tag.name = fmt::format("{} (#{})", animation_.path, entity);
    auto& sprite = reg.emplace<Sprite>(entity);
    auto& transform = reg.emplace<Transform>(entity);
    auto& anim = reg.emplace<Animator>(entity);

    auto clip = Engine::GetDefaultResource<AnimationSystem>()->Get(animation_.path);
    AssignSpriteTexture(sprite, clip->frames[0].textureName);
    AnimatorPlay(anim, animation_.path);
}

void Finder::OnRequestReloadingStructure()
{
    m_Structure.reset(new Folder());

    auto& textures = Engine::Res<Texture>();

    m_Structure->m_Nodes["textures"] = new Folder();
    for (auto& [k, v] : textures)
    {
        m_Structure->m_Nodes["textures"]->AddPath(k, k);
    }

    auto& animations = Engine::Res<Animation>();
    m_Structure->m_Nodes["animations"] = new Folder();
    for (auto& [k, v] : animations)
    {
        m_Structure->m_Nodes["animations"]->AddPath(k, k);
    }
}


void Finder::SpinUp()
{
    auto& dispatch = Engine::Dispatcher();

    dispatch.sink<AssetCreateRequest<Entity>>().connect<&Finder::OnRequestCreateEntity>(this);
    dispatch.sink<AssetCreateRequest<Texture>>().connect<&Finder::OnRequestCreateTexture>(this);
    dispatch.sink<AssetCreateRequest<Animation>>().connect<&Finder::OnRequestCreateAnimation>(this);

    dispatch.sink<AssetLoadRequest<PathNode>>().connect<&Finder::OnRequestReloadingStructure>(this);
	dispatch.sink<ToolMenuRender>().connect<&Finder::OnToolMenuRender>(this);
    dispatch.sink<GUIRender>().connect<&Finder::OnGUIRender>(this);

    dispatch.trigger<AssetLoadRequest<PathNode>>();
}


void Finder::Run()
{
    m_EntityQueue.Dispatch();
    m_TextureQueue.Dispatch();
    m_AnimationQueue.Dispatch();
}


void Finder::WindDown()
{
    auto& dispatch = Engine::Dispatcher();

    dispatch.sink<AssetCreateRequest<Entity>>().connect<&Finder::OnRequestCreateEntity>(this);
    dispatch.sink<AssetCreateRequest<Texture>>().disconnect<&Finder::OnRequestCreateTexture>(this);
    dispatch.sink<AssetCreateRequest<Animation>>().disconnect<&Finder::OnRequestCreateAnimation>(this);

    dispatch.sink<AssetLoadRequest<PathNode>>().disconnect<&Finder::OnRequestReloadingStructure>(this);
    dispatch.sink<ToolMenuRender>().disconnect<&Finder::OnToolMenuRender>(this);
    dispatch.sink<GUIRender>().disconnect<&Finder::OnGUIRender>(this);
}


Folder::~Folder() {
    if (!m_Nodes.empty())
    {
        for (auto [k, _] : m_Nodes)
        {
            delete m_Nodes[k];
        }
        m_Nodes.clear();
    }
}


void Folder::AddPath(String path_, String fullPath_)
{
	UInt64 colonIndex = path_.find(':');

	if (colonIndex == std::string::npos)
	{
		m_Nodes[path_] = new FolderLeaf(fullPath_);
	}
	else
	{
		auto key = path_.substr(0, colonIndex);
		auto rest = path_.substr(colonIndex + 1);

		if (!m_Nodes.contains(key))
		{
			m_Nodes[key] = new Folder();
		}

		m_Nodes[key]->AddPath(rest, fullPath_);
	}
}

void FolderLeaf::AddPath(String path_, String fullPath_) {}
