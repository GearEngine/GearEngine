#pragma once
#include "Gear/Core/Core.h"
#include "Layer.h"

namespace Gear{

	class GEAR_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		void clear()
		{
			for (int i = 0; i < m_Layers.size(); ++i)
			{
				delete m_Layers[i];
			}
			m_Layers.clear();
		}

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	public:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}


