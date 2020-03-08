#pragma once

#include "Scene.h"

namespace Gear {

	class SceneManager
	{
	private:
		SceneManager();
		~SceneManager();

	public:
		void AddScene(const std::string& name, Scene* scene);
		void EraseScene(const std::string& sceneName);

		void ScenePush(const std::string& name);
		void ScenePop();

		void changeScene(const std::string& name);

		inline Scene* GetCurrentScene() { return m_CurrentScene; }

		inline static SceneManager* Get() {
			if (s_Instance == nullptr)
				return s_Instance = new SceneManager;
			return s_Instance;
		}
		inline static void Destory() {
			if (s_Instance != nullptr)
				delete s_Instance;
		}
	private:
		std::unordered_map<std::string, Scene*> m_SceneStoradge;
		std::stack<Scene*> m_SceneStack;
		Scene* m_CurrentScene;
		static SceneManager* s_Instance;
	};


}