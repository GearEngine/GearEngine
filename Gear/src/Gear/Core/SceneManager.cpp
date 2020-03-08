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

	void SceneManager::AddScene(const std::string& name, Scene * scene)
	{
		auto target = m_SceneStoradge.find(name);
		if (target == m_SceneStoradge.end())
		{
			m_SceneStoradge.insert({ name, scene });
		}
		else
		{
			GR_CORE_TRACE("%s Scene aleady Exist!", name);
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
			GR_CORE_TRACE("%s Scene dosn't Exist!", name);
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
			GR_CORE_TRACE("%s Scene is aleady Current Scene!", name);
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