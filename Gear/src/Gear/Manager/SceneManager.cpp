#include "grpch.h"
#include "SceneManager.h"

namespace Gear {

	SceneManager* SceneManager::s_Instance = nullptr;

	SceneManager::SceneManager()
	{
	}

	SceneManager::~SceneManager()
	{
		while (!m_SceneStack.empty())
		{
			m_SceneStack.pop();
		}

		for (auto scene : m_SceneStoradge)
		{
			delete scene.second;
		}
		m_SceneStoradge.clear();
	}

	void SceneManager::AddScene(Scene * scene)
	{
		auto name = scene->GetName();
		auto target = m_SceneStoradge.find(name);
		if (target == m_SceneStoradge.end())
		{
			m_SceneStoradge.insert({ name, scene });
		}
		else
		{
			GR_CORE_TRACE("{0} Scene aleady Exist!", name);
		}
	}

	bool SceneManager::isSceneExist(const std::string& name)
	{
		auto scene = m_SceneStoradge.find(name);
		if (scene != m_SceneStoradge.end())
		{
			return true;
		}
		return false;
	}

	Scene * SceneManager::GetScene(const std::string & name)
	{
		auto scene = m_SceneStoradge.find(name);
		if (scene != m_SceneStoradge.end())
		{
			return scene->second;
		}
		else
		{
			GR_CORE_TRACE("{0} Scene dosn't Exist!", name);
			return nullptr;
		}
	}

	void SceneManager::EraseScene(const std::string & name)
	{
		auto scene = m_SceneStoradge.find(name);
		if (scene != m_SceneStoradge.end())
		{
			delete scene->second;
			m_SceneStoradge.erase(scene);
		}
		else
		{
			GR_CORE_TRACE("{0} Scene dosn't Exist!", name);
		}
	}

	void SceneManager::ScenePush(const std::string& name)
	{
		auto scene = m_SceneStoradge.find(name);
		if (m_CurrentScene != scene->second)
		{
			m_SceneStack.push(scene->second);
			m_CurrentScene = m_SceneStack.top();
		}
		else
		{
			GR_CORE_TRACE("{0} Scene is aleady Current Scene!", name);
		}

	}

	void SceneManager::ScenePop()
	{
		if (m_SceneStack.size() != 1) 
		{
			m_SceneStack.pop();
			m_CurrentScene = m_SceneStack.top();
		}
		else
		{
			GR_CORE_TRACE("SceneStack has only one Scene!");
		}
	}

	void SceneManager::changeScene(const std::string& name)
	{
		auto scene = m_SceneStoradge.find(name);
		if (scene != m_SceneStoradge.end())
		{
			if (m_CurrentScene != scene->second)
			{
				m_CurrentScene = scene->second;
			}
		}
	}

}