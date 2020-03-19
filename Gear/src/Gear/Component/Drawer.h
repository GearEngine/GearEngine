#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include "Gear/Renderer/Texture.h"
#include "Texturer.h"

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
		virtual void Render() override;
		void RenderAnimation();
		void RenderTexutre();

		glm::mat4 m_Translate;
		glm::vec4 m_TintColor;

		Ref<class Animation2D> m_Animation;

		BlendType::Type type;
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_Blending;
		Ref<Texture2D> m_Mask;

		friend class EntitySystem;
	};


}
