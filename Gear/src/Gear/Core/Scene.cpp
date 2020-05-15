#include "grpch.h"
#include "Scene.h"

namespace Gear {

	Scene::Scene(const std::string& name)
		: m_Name(name)
	{
	}

	Scene::~Scene()
	{
		m_LayerStack.clear();
	}

	void Scene::PushLayer(Layer * layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Scene::PushOverlay(Layer * overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Scene::PopLayer(Layer * layer)
	{
		m_LayerStack.PopLayer(layer);
	}

	void Scene::PopOverlay(Layer * overlay)
	{
		m_LayerStack.PopOverlay(overlay);
	}

}