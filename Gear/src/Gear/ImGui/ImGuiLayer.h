#pragma once
#include "Gear/Core/Layer.h"

#include "Gear/Events/KeyEvent.h"
#include "Gear/Events/MouseEvent.h"
#include "Gear/Events/ApplicationEvent.h"

namespace Gear {

	class GEAR_API 
		ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};

}