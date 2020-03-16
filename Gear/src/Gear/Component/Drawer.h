#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include "Gear/Renderer/Texture.h"

namespace Gear {

	class Drawer : public Component
	{
	private:
		virtual void Render() = 0;
	};

	class Drawer2D : public Drawer
	{
	public:
		virtual ~Drawer2D();

	private:

		virtual void Update(Timestep ts) override;
		void Render();

		glm::mat4 m_Translate;
		glm::vec4 m_TintColor;
		Ref<class Animation2D> m_Animation;

		friend class EntitySystem;
	};


}
