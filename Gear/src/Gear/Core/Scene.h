#pragma once

#include "LayerStack.h"

namespace Gear {

	class Scene 
	{
	public:
		Scene(const std::string& name);
		~Scene();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);
		
		inline std::string GetName() const { return m_Name; }

		std::vector<Layer*>::iterator begin() { return m_LayerStack.begin(); }
		std::vector<Layer*>::iterator end() { return m_LayerStack.end(); }

	private:
		std::string m_Name;
		LayerStack m_LayerStack;
	};
}